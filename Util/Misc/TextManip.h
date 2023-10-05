//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	TextManip
//------------------------------------------------------------------------------
RefString getFileExtension(const char* fileName);
size_t findLastSlash(const char* fileName);
size_t findLastDot(const char* fileName);
RefString plainName(const char* fileName);
RefString directoryPath(const char* filePath);