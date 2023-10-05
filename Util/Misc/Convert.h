//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"
#include "../Windows/Rect.h"

#include <list>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Convert
//------------------------------------------------------------------------------
std::vector<std::string> convertToStringArray(const char* input, const char* seperator = ",");
std::vector<RefString> convertToRefStringArray(const char* input, const char* seperator = ",");
std::vector<int> convertToIntArray(const char* input, const char* seperator);
std::list<std::string> convertToStringList(const char* input, const char* seperator = ",");
std::list<RefString> convertToRefStringList(const char* input, const char* seperator = ",");
std::list<int> convertToIntList(const char* input, const char* seperator);
int convertToInt(const char* input);
float convertToFloat(const char* input);
bool convertToBool(const char* input);
Rect convertToRect(const char* input);
Vector2i convertToVector2i(const char* input);
RefStringW convertToW(const RefString&);


struct StringMap
{
	const char* text;
	int value;
};

int convertToInt(const char* input, const StringMap* stringMap, size_t stringMapCount);