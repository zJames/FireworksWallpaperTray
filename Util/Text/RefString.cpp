//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "RefString.h"

#include <string.h>
////////////////////////////////////////////////////////////////////////////////


namespace RefStringHelper
{
	//------------------------------------------------------------------------------
	//	RefStringHelper
	//------------------------------------------------------------------------------
	size_t length(const char* text)
	{
		if (text == NULL)
		{
			return 0;
		}

		return strlen(text);
	}

	size_t length(const wchar_t* text)
	{
		if (text == NULL)
		{
			return 0;
		}

		return wcslen(text);
	}

	int compare(const char* first, const char* second)
	{
		return strcmp(first, second);
	}

	int compare(const wchar_t* first, const wchar_t* second)
	{
		return wcscmp(first, second);
	}
} //end namespace RefStringHelper
