//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Bitmap.h"

#include "../Core/File.h"
#include "../Misc/SwapT.h"
#include "../Pointers/ScopedPtr.h"

#include <tchar.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Bitmap
//------------------------------------------------------------------------------
typedef struct _DIBData
{
	BITMAPINFOHEADER InfoHeader;
} DIBData;


template<class T>
void clearImage(Bitmap& image, T color)
{
	for (int y = 0; y < image.height(); ++y)
	{
		T* linePtr = (T*)image.linePtr(y);

		for (int x = 0; x < image.width(); ++x, ++linePtr)
		{
			*linePtr = color;
		}
	}
}


Bitmap::Bitmap(const Bitmap& other)
{
	initBitmap();

	setSize(other.mWidth, other.mHeight, other.mFormat);

	if (other.channels() == 1)
	{
		RGBQUAD colors[256];

		other.palette(0, 256, colors);

		setPalette(0, 256, colors);
	}

	other.drawAll(drawSurface());
}


void Bitmap::clear(const DWORD color)
{
	switch (mChannels)
	{
	case 1:
		clearImage<BYTE>(*this, (BYTE)color);
		break;

	case 2:
		clearImage<WORD>(*this, (WORD)color);
		break;

	case 3:
		{
			const RGBQUAD* qColor = (const RGBQUAD*)(&color);
			RGBTRIPLE tColor;

			tColor.rgbtRed = qColor->rgbRed;
			tColor.rgbtGreen = qColor->rgbGreen;
			tColor.rgbtBlue = qColor->rgbBlue;

			clearImage<RGBTRIPLE>(*this, tColor);
		}
		break;

	case 4:
		clearImage<DWORD>(*this, color);
		break;
	}
}


// Reads in the colour map, returns the number of bytes of colour map read, or -1 for error.
int Bitmap::readColorMap(const BYTE* pData, const BITMAPINFOHEADER *infoheader)
{
	RGBQUAD ColorMap[256];

	/* Calculate the number of colors there should be in the colour map. */
	/* This will be in ClrUsed.  If ClrUsed is zero then the number of */
	/* colors is just 1 ^ bitsperpixel. */

	unsigned int ColorMapSize = infoheader->biClrUsed;

	if (infoheader->biClrUsed == 0)
		ColorMapSize = 1 << infoheader->biBitCount;

	if (ColorMapSize > 256)
		return -1;

	if (ColorMapSize)
	{
		memcpy(ColorMap, pData, sizeof(ColorMap[0]) * ColorMapSize);

		// Set the palette in the bitmap, or something like that.
		setPalette(0, ColorMapSize, ColorMap);
	}

	return ColorMapSize * sizeof(ColorMap[0]);
}


int Bitmap::decompressToBitmap(const char *CompressedData, int DataLength)
{
	const char *DataEnd = CompressedData + DataLength;
	const char *NearEnd = DataEnd - 2;	// Handy.

	for (int y = 0; y < height(); y++)
	{
		int x = 0;
		unsigned char *Output = linePtr(height() - y - 1);

		while (x < width())
		{
			unsigned char Input;
			// Make sure I have at least two bytes left.
			if (CompressedData > NearEnd)
			{
				return 0;	// Not enough data left.
			}

			Input = *CompressedData++;

			if (Input == 0)
			{
				Input = *CompressedData++;

				switch (Input)
				{
				case 0:
				case 1:
				case 2:
					return 0;

				default:
					if (x + Input > width())
					{
						return 0;
					}

					if (CompressedData + Input >= DataEnd)
					{
						return 0;
					}

					memcpy(Output + x, CompressedData, Input);

					x += Input;
					CompressedData += Input;

					if ((Input & 1) != 0)
						CompressedData++;

					break;
				}
			}
			else
			{
				if (x + Input > width())
				{
					return 0;
				}

				memset(Output + x, *CompressedData++, Input);

				x += Input;
			}
		}

		if (CompressedData <= NearEnd && CompressedData[0] == 0 && CompressedData[1] == 0)
		{
			CompressedData += 2;	// Skip over the end of line data.
		}
		else
		{
			// Out of data or missing end of line stuff.
			return 0;
		}
	}

	if (CompressedData <= NearEnd && CompressedData[0] == 0 && CompressedData[1] == 1)
	{
		CompressedData += 2;
	}
	else
	{
		// Out of data or missing end of bitmap stuff.
		return 0;
	}

	return height();
}


bool Bitmap::readBMPLine(const BYTE* pData, int y, int* offset)
{
	// We have to flip the y-coordinate because our bitmap class
	// puts line zero at the top of the screen, and the BMP format
	// on disk does the opposite.
	unsigned char *LinePtr = linePtr(height() - 1 - y);
	unsigned int BytesPerLine = ((width() * channels() + 3) & ~3);
	unsigned int UsedBytes = width() * channels();
	unsigned int Padding = BytesPerLine - UsedBytes;
	// Read the precise number of bytes that the line requires into the bitmap.
	// Don't read the padding bytes, because the in memory alignment requirements
	// may vary - we don't want our reading code to depend on our bitmap class
	// implementation.

	memcpy(LinePtr, pData + (*offset), UsedBytes);

	*offset += BytesPerLine;

	return true;
}


bool Bitmap::internalLoadBMPFile(const BYTE* pData)
{
	const BITMAPFILEHEADER* fileheader = (const BITMAPFILEHEADER*)pData;
	const BITMAPINFOHEADER* infoheader = (const BITMAPINFOHEADER*)(pData + sizeof(BITMAPFILEHEADER));

	// Check the type field to make sure we have a BMP file.
	if (fileheader->bfType != ('M' * 256 + 'B'))
		return false;

	// Sanity check the info header.
	if (infoheader->biSize != sizeof(BITMAPINFOHEADER))
		return false;

	if (infoheader->biPlanes != 1)
		return false;

	// These are the only depths I'm interested in handling. 1 and 4 bits per pixel
	// are also legal, but too much hassle.
	if (infoheader->biBitCount != 8
		&& infoheader->biBitCount != 24
		&& infoheader->biBitCount != 32)
	{
		return false;
	}

	if (!setSize(
		infoheader->biWidth,
		infoheader->biHeight,
		format(infoheader->biBitCount / 8)))
	{
		return false;
	}

	// Read the color map, if any.
	if (infoheader->biBitCount == 8
		&& readColorMap(
			pData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
			infoheader) <= 0)
	{
		// Illegal ColorMap value
		return false;
	}

	const BYTE* bmpData = pData + fileheader->bfOffBits;

#ifndef MOBILE_APP
	// Read in the bitmap data.
	if (infoheader->biCompression == BI_RLE8)
	{
		// We only support 8-bit RLE8 files - logically enough.
		if (infoheader->biBitCount != 8)
			return false;

		// Decompress.
		const int iNumLines =
			decompressToBitmap((char *)bmpData, infoheader->biSizeImage);

		if (iNumLines != height())
			return false;
	}
	else
#endif
		if (infoheader->biCompression == BI_RGB)
		{
			int offset = 0;

			for (int y = 0; y < infoheader->biHeight; ++y)
			{
				if (!readBMPLine(bmpData, y, &offset))
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

	initDerivedData();

	return true;
}


bool Bitmap::loadData(const char* data)
{
	if (data[0] == 'B'
		&& data[1] == 'M')
	{
		return internalLoadBMPFile((const BYTE*)data);
	}

	int *info = (int*)data;
	int x;

	if (!data)
		return false;

	setSize(info[0], info[1], format(info[2]));

	for (x = 0; x < info[1]; x++)
	{
		memcpy(linePtr(x), data + x * info[0] * info[2], info[0] * info[2]);
	}

	initDerivedData();

	return true;
}


bool Bitmap::loadBMPFile(const char* szFileName)
{
	freeBitmap();

	File file(szFileName, File::eOption_Read_Binary);

	if (!file.isValid())
		return false;

	const size_t fileSize = file.size();

	ScopedArray<BYTE> data(new BYTE[fileSize]);

	if (file.read(data.get(), fileSize))
	{
		if (internalLoadBMPFile(data.get()))
		{
			return true;
		}
	}

	freeBitmap();

	return false;
}


bool Bitmap::loadResource(LPCSTR name, HINSTANCE hInst)
{
	freeBitmap();

	HBITMAP image =
		(HBITMAP)LoadImage(
			hInst,
			name,
			IMAGE_BITMAP,
			0,
			0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (image == NULL)
		return false;

	BITMAP info;

	GetObject(image, sizeof(info), &info);

	setSize(info.bmWidth, info.bmHeight, format(info.bmPlanes));

	HDC tempDC = CreateCompatibleDC(NULL);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(tempDC, image);

	if (info.bmPlanes == 1)
	{
		RGBQUAD rgb[256];

		GetDIBColorTable(tempDC, 0, 256, rgb);

		setPalette(0, 256, rgb);
	}

	BitBlt(mBitmapDC, 0, 0, mWidth, mHeight, tempDC, 0, 0, SRCCOPY);

	SelectObject(tempDC, oldBitmap);
	DeleteDC(tempDC);
	DeleteObject(image);

	return true;
}


Bitmap& Bitmap::operator =(const Bitmap& other)
{
	setSize(other.width(), other.height(), other.mFormat);

	other.drawAll(drawSurface());

	return *this;
}


void Bitmap::initBitmap()
{
	// Set mWidth, mHeight and mChannels to -1 so that we can tell
	// whether this initialization function has been called. This
	// helps us detect bugs.
	mWidth = mHeight = mChannels = -1;
	mSurfaceBits = 0;
	mBitmapNew = 0;
	mBitmapDC = 0;
	mBitmapMonochrome = 0;
}


void Bitmap::freeBitmap()
{
	if (mBitmapDC)
	{
		if (mBitmapMonochrome)
		{
			// Select the stock 1x1 monochrome bitmap back in
			HBITMAP hBitmapOld = (HBITMAP)SelectObject(mBitmapDC, mBitmapMonochrome);
		}

		if (mBitmapNew)
			DeleteObject(mBitmapNew);

		if (mBitmapDC)
			DeleteDC(mBitmapDC);
	}

	initBitmap();
}


// Theoretically we should be paying attention to this:
// From the CreateDIBSection help:
// Windows NT: You need to guarantee that the GDI subsystem has completed any
// drawing to a bitmap created by CreateDIBSection before you draw to the
// bitmap yourself. Access to the bitmap must be synchronized. Do this by
// calling the GdiFlush function. This applies to any use of the pointer
// to the bitmap’s bit values, including passing the pointer in calls to
// functions such as SetDIBits. 
unsigned char *Bitmap::linePtr(int y) const
{
	return mLineZeroPointer + y * mStride;
}


unsigned char *Bitmap::pixelPtr(int x, int y) const
{
	return linePtr(y) + x * mChannels;
}


void Bitmap::initDerivedData()
{
	// Here is where we have to calculate the actual number of bytes per line,
	// respecting the Win32 rules for bitmap allocations. When we requested
	// the bitmap we specified a width, which Windows will have padded to
	// a multiple of four bytes - so we'd better allow for that.
	mBytesPerLine = (mWidth * mChannels + 3) & ~3;
	// Calculate the address of line zero - defined here as the line that is
	// at the top when the bitmap is copied to the screen. This is the last
	// line in memory.
	mLineZeroPointer = mSurfaceBits + (mHeight - 1) * mBytesPerLine;
	// Calculate the distance from line to line to be consistent with
	// mLineZeroPointer.
	mStride = -mBytesPerLine;
}

void Bitmap::setPalette(int StartColor, int NumColors, const RGBQUAD* pColors)
{
	SetDIBColorTable(mBitmapDC, StartColor, NumColors, pColors);
}

bool Bitmap::palette(int StartColor, int NumColors, RGBQUAD* pColors) const
{
	// Zero the destination array to make sure all bytes get cleared.
	memset(pColors, 0, sizeof(RGBQUAD) * NumColors);

	const int GotCount = GetDIBColorTable(drawSurface(), StartColor, NumColors, pColors);

	return GotCount == NumColors;
}


static const int determineChannels(Bitmap::Format fmt)
{
	switch (fmt)
	{
	case Bitmap::F_INDEXED:
		return 1;
	case Bitmap::F_R8G8B8:
		return 3;

#ifndef MOBILE_APP
	case Bitmap::F_R5G5B5:
		return 2;
#endif

	case Bitmap::F_R5G6B5:
		return 2;

	default:
	case Bitmap::F_A8R8G8B8:
		return 4;
	}
}


bool Bitmap::setSize(int width, int height, Format fmt)
{
	int channels = determineChannels(fmt);

	// Make sure that the InitBitmap() function was called before calling
	// SetSize() for the first time.
	// This function doesn't support allocating zero size or negative
	// size bitmaps.
	freeBitmap();

	mWidth = width;
	mHeight = height;
	mChannels = channels;
	mFormat = fmt;

	// Set up an optimal bitmap
	HDC TempDC = CreateDC(_T("Display"), 0, 0, 0);

	if (TempDC)
	{
#ifndef MOBILE_APP
		if (fmt != F_R5G5B5)
#endif
		{
			DIBData	Info;
			memset(&Info, 0, sizeof(Info));
			Info.InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			Info.InfoHeader.biWidth = width;
			Info.InfoHeader.biHeight = height;
			Info.InfoHeader.biPlanes = 1;
			Info.InfoHeader.biBitCount = channels * 8;
			Info.InfoHeader.biCompression = BI_RGB;
			// Support for 256 color bitmaps.
			Info.InfoHeader.biClrUsed = (channels == 1 ? 256 : 0);

			mBitmapNew =
				CreateDIBSection(
					TempDC,
					(BITMAPINFO *)&Info,
					DIB_RGB_COLORS,
					(void **)&mSurfaceBits,
					0,
					0L);
		}
#ifndef MOBILE_APP
		else
		{
			BITMAPV4HEADER info;

			memset(&info, 0, sizeof(info));

			info.bV4Size = sizeof(BITMAPV4HEADER);
			info.bV4BitCount = 16;
			info.bV4Planes = 1;
			info.bV4Width = width;
			info.bV4Height = height;
			info.bV4V4Compression = BI_BITFIELDS;
			info.bV4AlphaMask = 0x8000;
			info.bV4RedMask = 0x7C00;
			info.bV4GreenMask = 0x03E0;
			info.bV4BlueMask = 0x001F;

			mBitmapNew =
				CreateDIBSection(
					TempDC,
					(BITMAPINFO *)&info,
					DIB_RGB_COLORS,
					(void **)&mSurfaceBits,
					0,
					0L);
		}
#endif
		if (mBitmapNew)
		{
			mBitmapDC = CreateCompatibleDC(TempDC);

			if (mBitmapDC)
				mBitmapMonochrome = (HBITMAP)SelectObject(mBitmapDC, mBitmapNew);
		}

		DeleteDC(TempDC);
	}

	// If the allocation failed, clean up.
	if (!mBitmapMonochrome)
	{
		freeBitmap();
	}

	initDerivedData();

	return hasBitmap();
}


void Bitmap::draw(HDC destDC, int destx, int desty) const
{
	if (!hasBitmap())
		return;

	BitBlt(destDC, destx, desty, width(), height(), mBitmapDC, 0, 0, SRCCOPY);
}

void Bitmap::draw(HDC destDC, int destx, int desty, int destwidth, int destheight, int sourcex, int sourcey) const
{
	if (!hasBitmap())
		return;

	StretchBlt(
		destDC,
		destx + ((destwidth < 0) ? -destwidth : 0),
		desty + ((destheight < 0) ? -destheight : 0),
		destwidth,
		destheight,
		drawSurface(),
		sourcex,
		sourcey,
		mWidth,
		mHeight,
		SRCCOPY);
}


void Bitmap::drawAll(Bitmap& dest) const
{
	draw(dest, 0, 0, dest.width(), dest.height(), 0, 0);;
}


void Bitmap::drawAll(HDC destDC) const
{
	if (!hasBitmap())
	{
		return;
	}

	draw(destDC, 0, 0, width(), height(), 0, 0);
}


bool Bitmap::writeBMPLine(File& file, int y) const
{
	// A buffer to use to write the padding bytes from.
	const char paddingBuffer[3] = { 0 };
	// We have to flip the y-coordinate because our bitmap class
	// puts line zero at the top of the screen, and the BMP format
	// on disk does the opposite.
	const unsigned char* line = linePtr(height() - 1 - y);
	const unsigned int bytesPerLine = ((width() * channels() + 3) & ~3);
	const unsigned int usedBytes = width() * channels();
	const unsigned int padding = bytesPerLine - usedBytes;

	// Read the precise number of bytes that the line requires into the bitmap.
	// Don't read the padding bytes, because the in memory alignment requirements
	// may vary - we don't want our reading code to depend on our bitmap class
	// implementation.
	if (!file.write(line, usedBytes))
	{
		return false;
	}

	// Write out any padding bytes.
	if (padding > 0 && !file.write(paddingBuffer, padding))
	{
		return false;
	}

	return true;
}


bool Bitmap::internalSaveBMPFile(File& file) const
{
	BITMAPFILEHEADER fileheader = { 0 };
	BITMAPINFOHEADER infoheader = { 0 };

	fileheader.bfType = 'M' * 256 + 'B';
	fileheader.bfOffBits = sizeof(fileheader) + sizeof(infoheader);

	if (channels() == 1)
	{
		fileheader.bfOffBits += 256 * sizeof(RGBQUAD);
	}

	infoheader.biSize = sizeof(infoheader);
	infoheader.biPlanes = 1;
	infoheader.biBitCount = channels() * 8;
	infoheader.biWidth = width();
	infoheader.biHeight = height();
	infoheader.biCompression = BI_RGB;	// I'm not compressing it!
	// I don't need to set biSizeImage or biClrUsed.

	// Read the file header.
	if (!file.write(&fileheader, sizeof(fileheader)))
	{
		return false;
	}

	// Read the info header.
	if (!file.write(&infoheader, sizeof(infoheader)))
	{
		return false;
	}

	// Read the color map, if any.
	if (infoheader.biBitCount == 8)
	{
		RGBQUAD	pal[256];

		if (!palette(0, 256, pal))
		{
			return false;
		}

		if (!file.write(pal, sizeof(pal)))
		{
			return false;
		}
	}

	for (int y = 0; y < infoheader.biHeight; ++y)
	{
		if (!writeBMPLine(file, y))
		{
			return false;
		}
	}

	return true;
}


bool Bitmap::saveBMPFile(const char* szFileName) const
{
	if (!hasBitmap())
	{
		return false;
	}

	File file(szFileName, File::eOption_Write_Binary);

	// This is the cleanest way of making sure that the file always gets
	// closed.
	if (file.isValid())
	{
		return internalSaveBMPFile(file);
	}

	return false;
}


void Bitmap::rgbaFpxToBgra()
{
	for (int i = 0; i < mHeight; ++i)
	{
		BYTE* ptr = linePtr(i);

		for (int j = 0; j < mWidth; j++)
		{
			BYTE r = ptr[0];
			BYTE g = ptr[1];
			BYTE b = ptr[2];
			BYTE a = ptr[3];
			ptr[2] = (r * a + 1) >> 8;
			ptr[1] = (g * a + 1) >> 8;
			ptr[0] = (b * a + 1) >> 8;
			ptr += 4;
		}
	}
}


Bitmap::Format Bitmap::format(int channels)
{
	switch (channels)
	{
	case 1:
		return F_INDEXED;

	case 2:
#ifdef MOBILE_APP
		return F_R5G6B5;
#else
		return F_R5G5B5;
#endif

	case 3:
		return F_R8G8B8;

	default:
		return F_A8R8G8B8;
	}
}


void Bitmap::swap(Bitmap& other)
{
	swapT(mBytesPerLine, other.mBytesPerLine);
	swapT(mChannels, other.mChannels);
	swapT(mHeight, other.mHeight);
	swapT(mStride, other.mStride);
	swapT(mWidth, other.mWidth);
	swapT(mSurfaceBits, other.mSurfaceBits);
	swapT(mLineZeroPointer, other.mLineZeroPointer);
	swapT(mBitmapDC, other.mBitmapDC);
	swapT(mBitmapNew, other.mBitmapNew);
	swapT(mBitmapMonochrome, other.mBitmapMonochrome);
	swapT(mFormat, other.mFormat);
}