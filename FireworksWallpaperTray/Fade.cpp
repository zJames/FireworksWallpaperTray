//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Fade.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Fade
//------------------------------------------------------------------------------
static BYTE fadeToBlack(const BYTE color)
{
	return (color * 220) / 256;
}


static WORD fadeToBlack(const WORD pixel)
{
	WORD ret = 0;

	for (int i = 0; i < 3; ++i)
	{
		const int shift = (i * 5);
		WORD val = (ret >> shift) & 0x1F;

		ret |= ((val * 220) / 256) << shift;
	}

	return ret;
}


static void fadeToBlack8(BYTE* pixels, int stride)
{
	stride = abs(stride);

	for (int x = 0; x < stride; ++x, ++pixels)
	{
		*pixels = fadeToBlack(*pixels);
	}
}


static void fadeToBlack555(WORD* pixels, int stride)
{
	stride = abs(stride);

	for (int x = 0; x < stride; ++x, ++pixels)
	{
		*pixels = fadeToBlack(*pixels);
	}
}


void fadeToBlack(Bitmap& image)
{
	if (image.channels() == 2)
	{
		for (int y = 0; y < image.height(); ++y)
		{
			fadeToBlack555((WORD*)image.linePtr(y), image.stride());
		}
	}
	else if (image.channels() == 4)
	{
		for (int y = 0; y < image.height(); ++y)
		{
			fadeToBlack8(image.linePtr(y), image.stride());
		}
	}
}