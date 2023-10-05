//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Bitmap.h"
#include "CycleBitmap.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	LoadImage
//------------------------------------------------------------------------------
namespace Graphics
{
	void LoadImage(const char* txtr, Bitmap& image);
	void LoadImage(const char* txtr, CycleBitmap& image);
	void LoadImage(const char* txtr, Bitmap& image, Bitmap::Format format);
	void LoadImage(const void* data, size_t dataSize, Bitmap& image);
	void LoadImage(const void* data, size_t dataSize, Bitmap& image, Bitmap::Format format);
	void LoadImage(LPCSTR resourceId, HINSTANCE hInst, Bitmap& image);
	void LoadImage(LPCSTR resourceId, HINSTANCE hInst, Bitmap& image, Bitmap::Format format);
	void ForceFormat(Bitmap& image, Bitmap::Format format);
} //end namespace Graphics