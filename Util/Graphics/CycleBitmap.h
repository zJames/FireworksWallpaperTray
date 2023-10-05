//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Bitmap.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//	CycleBitmap
//------------------------------------------------------------------------------
class CycleBitmap
{
public:
	struct Cycle
	{
		Cycle(int start, int end, int delay, bool fwd);

		int startColor;
		int endColor;
		int delay;
		bool forward;

		int curDelay;
	};

	typedef std::vector<Cycle> CycleList;

	CycleBitmap();
	CycleBitmap(const char* image, const char* cycles);

	void update();
	void updatePalette();
	void reset();
	void loadImage(const char* fileName);
	void loadCycles(const char* fileName);
	void loadPal(const char* fileName);

	void removeCycle(int);
	
	void saveCycles(const char* fileName);
	void savePal(const char* fileName);

	operator HDC() { return image; }

	int height() const { return image.height(); }
	int width() const { return image.width(); }
	bool hasBitmap() const { return image.hasBitmap(); }

	Bitmap image;
	CycleList cycles;
	RGBQUAD colors[256];
};