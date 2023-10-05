//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////

class Bitmap;

bool LoadPNG(const char* fileName, Bitmap& output);
bool LoadPNG(const unsigned char* data, size_t dataSize, Bitmap& output);
void SavePNG(const Bitmap& input, const char* fileName);