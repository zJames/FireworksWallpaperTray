//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ConvertToGrayMap.h"

#include "Bitmap.h"
#include "..\Math\Clamp.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
static BYTE grayVal(const RGBQUAD& rgb)
{
	//0.393, 0.769, 0.189
	const float fVal =
		(rgb.rgbRed * 0.393f) + (rgb.rgbGreen * 0.769f) + (rgb.rgbBlue * 0.189f);
	
	return BYTE(clamp(fVal, 0.0f, 255.0f));
}


//------------------------------------------------------------------------------
//	ConvertToGrayMap
//------------------------------------------------------------------------------
void convertToGrayMap(const Bitmap& input, Bitmap& output)
{
	output.setSize(input.width(), input.height(), Bitmap::F_INDEXED);

	RGBQUAD palette[256];

	input.palette(0, 256, palette);

	BYTE palToIdx[256] = {0};

	for (int i = 0; i < 256; ++i)
	{
		palToIdx[i] = grayVal(palette[i]);
	}

	for (int y = 0; y < input.height(); ++y)
	{
		BYTE* const inLine = input.linePtr(y);
		BYTE* outLine = output.linePtr(y);

		for (int x = 0; x < input.width(); ++x)
		{
			outLine[x] = palToIdx[inLine[x]];
		}
	}
}