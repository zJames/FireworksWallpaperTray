//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "LoadPNG.h"

#include "Bitmap.h"
#include "../Core/Debug.h"
#include "../Core/File.h"
#include "lodepng.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
RGBQUAD byteReorder(const RGBQUAD& input)
{
	RGBQUAD ret;
	char* src = (char*)&input;
	char* dst = (char*)&ret;

	dst[0] = src[2];
	dst[1] = src[1];
	dst[2] = src[0];
	dst[3] = src[3];

	return ret;
}


bool LoadInternal(unsigned error, Bitmap &output, unsigned width, unsigned height, std::vector<unsigned char> image, const char* fileName = "")
{
	//if there's an error, display it
	if (error)
	{
		LogDebugArgs("LoadPNG: decode error %d (file:%s): %s\n", error, fileName, lodepng_error_text(error));

		return false;
	}

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
	output.setSize(width, height, Bitmap::F_A8R8G8B8);

	RGBQUAD* source = (RGBQUAD*)&image[0];

	for (uint32_t y = 0; y < height; ++y)
	{
		RGBQUAD* destLine = (RGBQUAD*)output.linePtr(y);
		RGBQUAD* sourceLine = source + (y * width);

		//_memccpy(destLine, sourceLine, width, sizeof(RGBQUAD));

		for (uint32_t x = 0; x < width; ++x)
		{
			destLine[x] = byteReorder(sourceLine[x]);
		}
	}

	return true;
}

template<int Channels>
void convertForLodePNG(const Bitmap& input, std::vector<BYTE>& output)
{
	for (int y = 0; y < input.height(); ++y)
	{
		BYTE* pLine = input.linePtr(y);

		for (int x = 0; x < input.width(); ++x)
		{
			output.push_back(pLine[(x * Channels) + 2]);
			output.push_back(pLine[(x * Channels) + 1]);
			output.push_back(pLine[(x * Channels) + 0]);

			if (Channels == 4)
			{
				output.push_back(pLine[(x * Channels) + 3]);
			}
		}
	}
}


//------------------------------------------------------------------------------
//	Exported Functions
//------------------------------------------------------------------------------
bool LoadPNG(const char* fileName, Bitmap& output)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, fileName);

	return LoadInternal(error, output, width, height, image);
}


bool LoadPNG(const unsigned char* data, size_t dataSize, Bitmap& output)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	lodepng::State state;
	unsigned error = lodepng::decode(image, width, height, state, data, dataSize);

	return LoadInternal(error, output, width, height, image);
}


void SavePNG(const Bitmap& input, const char* fileName)
{
	std::vector<BYTE> output;

	switch (input.format())
	{
	case Bitmap::F_A8R8G8B8:
		{
			std::vector<BYTE> temp;

			convertForLodePNG<4>(input, temp);
			//lodepng_encode32(output, &outputSize, &temp[0], input.width(), input.height());
			lodepng::encode(output, temp, input.width(), input.height());

		}
		break;

	case Bitmap::F_R8G8B8:
		{
			std::vector<unsigned char> temp;

			convertForLodePNG<3>(input, temp);
			//lodepng_encode24(&output, &outputSize, input.dataPtr(), input.width(), input.height());
			lodepng::encode(output, temp, input.width(), input.height(), LodePNGColorType::LCT_RGB);
		}
		break;

	case Bitmap::F_R5G6B5:
	case Bitmap::F_R5G5B5:
	case Bitmap::F_INDEXED:
		{
			Bitmap temp;

			temp.setSize(input.width(), input.height(), Bitmap::F_R8G8B8);
			input.drawAll(temp);

			std::vector<BYTE> tempData;

			convertForLodePNG<3>(temp, tempData);

			//lodepng_encode24(&output, &outputSize, &tempData[0], input.width(), input.height());
			lodepng::encode(output, tempData, input.width(), input.height(), LodePNGColorType::LCT_RGB);
		}
		break;
	}

	if (output.size() > 0)
	{
		File outFile(fileName, File::eOption_Write_Binary);

		outFile.write(&output[0], output.size());
	}
}