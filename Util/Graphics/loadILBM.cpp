//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "loadILBM.h"

#include "CycleBitmap.h"
#include "../Core/File.h"
#include "../Pointers/ScopedPtr.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Defines
//------------------------------------------------------------------------------
#define BIT(n) (1 << (n))


//------------------------------------------------------------------------------
//	Local Enumerations
//------------------------------------------------------------------------------
enum Flags
{
	eFlag_rgb,
	eFlag_alpha,
	eFlag_zBuffer,

	eFlag_num
};


DWORD reverse(DWORD input)
{
	DWORD ret = 0;

	for (int i = 0; i < 4; ++i)
	{
		const DWORD val = (input >> (i * 8)) & 0xFF;
		ret |= val << ((3 - i) * 8);
	}

	return ret;
}


WORD reverse(WORD input)
{
	return (input >> 8) | (input << 8);
}


//------------------------------------------------------------------------------
//	Local Structs
//------------------------------------------------------------------------------
struct Header
{
	Header() : length(0) {}

	Header(BYTE* input)
	{
		type = reverse(*(DWORD*)(input + 0));
		length = reverse(*(DWORD*)(input + 4));

		process();
	}

	void process()
	{
		BYTE* b = (BYTE*)&type;

		for (int i = 0; i < sizeof(type); ++i)
			b[i] = toupper(b[i]);
	}

	union
	{
		DWORD type;
		char name[4];
	};

	DWORD length;
};


struct BMHDHeader
{
	WORD width;
	WORD height;
	WORD originX;
	WORD originY;
	BYTE bitplanes;
	BYTE maskingType;
	BYTE compressionType;
	BYTE padding;			//Should always be 0
	WORD transparencyType;
	BYTE xAspectRatio;
	BYTE yAspectRatio;
	WORD resolutionWidth;
	WORD resolutionHeight;
};


struct Chunk : public Header
{
	Chunk() : offset(-1) {}
	Chunk(BYTE* input, int ofs) : Header(input), offset(ofs) {}

	int offset;
};


class ILBM
{
public:
	ILBM(BYTE* input);

	Header header; //Main header

	Chunk body;
	Chunk cmap;
	Chunk bmhd;
	Chunk cbmp;
	std::vector<Chunk> crng;
};


class ColorRange
{
public:
	ColorRange(const BYTE* input)
	{
		input += sizeof(Header);
		input += 2; //skip padding

		cycleRate = reverse(*(const WORD*)input);
		input += 2;

		WORD flags = *(const WORD*)input;
		input += 2;

		isActive = (flags & 1) != 0;
		isReversed = (flags & 2) != 0;

		start = *input++;
		end = *input++;
	}

	bool isActive;
	bool isReversed;
	WORD cycleRate;
	BYTE start;
	BYTE end;
};


//------------------------------------------------------------------------------
//	ILBM
//------------------------------------------------------------------------------
ILBM::ILBM(BYTE* input)
	:
	header(input)
{
	int offset = sizeof(header) + 4;

	while (offset < (int)header.length)
	{
		Chunk test(input + offset, offset + sizeof(Header));

		switch (test.type)
		{
		case 'CBMP':
			cbmp = test;
			break;

		case 'BMHD':
			bmhd = test;
			break;

		case 'CMAP':
			cmap = test;
			break;

		case 'CRNG':
			crng.push_back(Chunk(input + offset, offset));
			break;

		case 'BODY':
			body = test;
			break;
		}

		offset += test.length + sizeof(Header);
	}
}


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
void readColors(const ILBM& header, Bitmap& newImage, const BYTE* data)
{
	RGBQUAD colors[256];

	const int numColors = min(header.cmap.length / 3, 256);

	const BYTE* clrData = data + header.cmap.offset;

	for (int i = 0; i < numColors; ++i)
	{
		colors[i].rgbRed = *clrData++;
		colors[i].rgbGreen = *clrData++;
		colors[i].rgbBlue = *clrData++;
	}

	newImage.setPalette(0, numColors, colors);
}


void decompRun(
	Bitmap& dest,
	const int y,
	const BYTE* data,
	int& offset)
{
	int idx = 0;
	BYTE* destBuf = dest.linePtr(y);

	while (idx < dest.width())
	{
		char code = *(char*)&data[offset++];

		if (code == -128)
		{
			continue;
		}
		else if (code >= 0)
		{
			while (idx < dest.width() && code >= 0)
			{
				destBuf[idx++] = data[offset++];
				--code;
			}
		}
		else
		{
			const BYTE val = data[offset++];

			while (idx < dest.width() && code <= 0)
			{
				destBuf[idx++] = val;
				++code;
			}
		}
	}
}


void readImage(const ILBM& header, Bitmap& dest, const BYTE* data)
{
	const BMHDHeader* bmhd = (BMHDHeader*)(data + header.bmhd.offset);

	const BYTE* body = data + header.body.offset;
	int offset = 0;
	const int numPixels = dest.width() * dest.height();
	const int planes = dest.channels() * 8;

	dest.clear(0);

	for (int y = 0; y < dest.height(); ++y)
	{
		decompRun(dest, y, body, offset);
	}
}


void readColorRanges(const ILBM& header, CycleBitmap& dest, const BYTE* data)
{
	if (header.crng.size() == 0)
		return;

	dest.cycles.clear();

	for (int i = 0; i < (int)header.crng.size(); ++i)
	{
		const ColorRange range(data + header.crng[i].offset);

		if (!range.isActive)
			continue;

		int start = range.start;
		int end = range.end;
		bool forward = !range.isReversed;

		if (range.start > range.end)
		{
			start = range.end;
			end = range.start;
			forward ^= true;
		}

		dest.cycles.push_back(
			CycleBitmap::Cycle(
				start,
				end,
				range.cycleRate / 400,
				forward));
	}
}


void loadInternal(const char* fileName, ScopedArray<BYTE>& data)
{
	if (fileName == NULL)
	{
		return;
	}

	File input;

	if (!input.open(fileName, File::eOption_Read_Binary))
	{
		return;
	}

	const size_t fileSize = input.size();

	data.set(new BYTE[fileSize]);

	input.read(data.get(), fileSize);
}


//------------------------------------------------------------------------------
//	loadILBM
//------------------------------------------------------------------------------
void loadILBM(const char* szFileName, Bitmap& dest)
{
	ScopedArray<BYTE> data;

	loadInternal(szFileName, data);

	if (data.get() == NULL)
	{
		return;
	}

	ILBM header(data.get());

	if (header.bmhd.offset < 0)
	{
		return;
	}

	const BMHDHeader* bmhd = (BMHDHeader*)(data.get() + header.bmhd.offset);

	if (bmhd->bitplanes != 8)
	{
		return;
	}

	dest.setSize(reverse(bmhd->width), reverse(bmhd->height), Bitmap::F_INDEXED);

	readColors(header, dest, data.get());
	readImage(header, dest, data.get());
}


void loadILBM(const char* szFileName, CycleBitmap& dest)
{
	ScopedArray<BYTE> data;

	loadInternal(szFileName, data);

	if (data.get() == NULL)
	{
		return;
	}

	ILBM header(data.get());

	if (header.bmhd.offset < 0)
	{
		return;
	}

	const BMHDHeader* bmhd = (BMHDHeader*)(data.get() + header.bmhd.offset);

	if (bmhd->bitplanes != 8)
	{
		return;
	}

	dest.image.setSize(reverse(bmhd->width), reverse(bmhd->height), Bitmap::F_INDEXED);

	readColors(header, dest.image, data.get());
	readImage(header, dest.image, data.get());
	readColorRanges(header, dest, data.get());

	dest.updatePalette();
}