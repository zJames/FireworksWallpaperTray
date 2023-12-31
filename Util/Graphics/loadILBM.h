//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <string>
////////////////////////////////////////////////////////////////////////////////


class Bitmap;
class CycleBitmap;


//------------------------------------------------------------------------------
//	loadILBM
//------------------------------------------------------------------------------
void loadILBM(const char* szFileName, Bitmap& dest);
void loadILBM(const char* szFileName, CycleBitmap& dest);