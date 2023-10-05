//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Parsing Functions
//------------------------------------------------------------------------------
bool isWhite(char c);
bool isWhiteOrNull(const char c);
bool comp(const char* first, const char* second, bool checkEnd = true);
const char* eatWhite(const char* start);
RefString getToken(const char*& start);
const char* find(const char* start, const char seek);
const char* parseOver(const char* start);
const char* skipAttrib(const char* start);
RefString findOffsets(const char* const start, std::vector<int>& offsets);
RefString readAttrib(const char* start);
