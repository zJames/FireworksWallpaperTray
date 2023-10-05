//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "LoadPCX.h"

#include "Bitmap.h"
#include "../Core/Debug.h"
#include "../Core/File.h"
#include "../Util.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Structs
//------------------------------------------------------------------------------
#pragma pack(push, 1) 

struct PCXHead
{
	BYTE		Manufacturer;
	BYTE		Version;
	BYTE		Encoding;
	BYTE		Bpp;
	WORD		Xmin, Ymin, Xmax, Ymax;
	WORD		HDpi;
	WORD		VDpi;
	BYTE		ColMap[48];
	BYTE		Reserved;
	BYTE		NumPlanes;
	WORD		Bps;
	WORD		PaletteInfo;
	WORD		HScreenSize;
	WORD		VScreenSize;
	BYTE		Filler[54];
};

#pragma pack(pop)


//------------------------------------------------------------------------------
//	Local Files
//------------------------------------------------------------------------------
static void readPalette(Bitmap& dest, File& input, const PCXHead& header)
{
	if (header.Version != 5 || dest.channels() != 1)
		return;
	
	BYTE byteHead = 0;

	fseek(input, 0, SEEK_END);
	fseek(input, -769, SEEK_CUR);

	if (!input.read(&byteHead, sizeof(byteHead)))
		return;

	RGBTRIPLE colors[256];

	if (byteHead != 12)  // Some Quake2 .pcx files don't have this byte for some reason.
	{
		fseek(input, -1, SEEK_CUR);
	}

	if (!input.read(colors, sizeof(colors)))
		return;

	RGBQUAD qcolors[256];

	for (int i = 0; i < ARRAY_LENGTH(colors); ++i)
	{
		qcolors[i].rgbBlue = colors[i].rgbtRed;
		qcolors[i].rgbGreen = colors[i].rgbtGreen;
		qcolors[i].rgbRed = colors[i].rgbtBlue;
	}

	dest.setPalette(0, 256, qcolors);
}


static void loadPCXInternal(Bitmap& dest, File& input)
{
	PCXHead header;

	input.read(&header, sizeof(header));

	dest.setSize(
		header.Xmax - header.Xmin + 1,
		header.Ymax - header.Ymin + 1,
		Bitmap::format(header.NumPlanes));

	const int scanlineSize = dest.channels() * header.Bps;

	BYTE* scanLine = new BYTE[scanlineSize];

	__try
	{
		for (int y = 0; y < dest.height(); ++y)
		{
			int x = 0;

			while (x < scanlineSize)
			{
				BYTE byteHead = 0;

				input.read(&byteHead, sizeof(byteHead));

				if ((byteHead & 0xC0) == 0xC0)
				{
					byteHead &= 0x3F;
					BYTE color;

					if (!input.read(&color, sizeof(color)))
					{
						__leave;
					}

					if (x + byteHead > scanlineSize)
					{
						__leave;
					}

					for (int i = 0; i < byteHead; i++)
					{
						scanLine[x++] = color;
					}
				}
				else
				{
					scanLine[x++] = byteHead;
				}
			}

			BYTE* linePtr = (BYTE*)dest.linePtr(y);

			//convert plane-separated scanline into index, rgb or rgba pixels.
			//there might be a padding byte at the end of each scanline...
			for (int x = 0; x < dest.width(); x++)
			{
				for(int c = 0; c < dest.channels(); ++c)
				{
					linePtr[x * dest.channels() + (dest.channels() - c - 1)] =
						scanLine[x + c * header.Bps];
				}
			}
		}

		readPalette(dest, input, header);
	}
	__finally
	{
		delete [] scanLine;
	}
}


//------------------------------------------------------------------------------
//	PCX Loader
//------------------------------------------------------------------------------
void loadPCX(const char* szFileName, Bitmap& dest)
{
	File input;

	if (!input.open(szFileName, File::eOption_Read_Binary))
	{
		LogDebugArgs("loadPCX.cpp unable to load file: %s\n", szFileName);
		return;
	}

	loadPCXInternal(dest, input);
}