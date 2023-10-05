//------------------------------------------------------------------------------
//  Copyright : (c) 2013
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "TextIterator.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
static bool comp(const char* str1, const char* str2)
{
	if (str1 == NULL || str2 == NULL)
		return str1 == str2;

	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return false;

		++str1;
		++str2;
	}

	return true;
}


//------------------------------------------------------------------------------
//	TextIterator
//------------------------------------------------------------------------------
TextIterator::TextIterator(TextType text, char splitOn)
	:
	TextIteratorBase(text, splitOn)
{
}

TextIterator::TextIterator(TextType text, const char* splitOn)
	:
	TextIteratorBase(text, splitOn)
{
}

TextIterator::TextIterator(TextType text, const SplitArray& splitOn)
	:
	TextIteratorBase(text, splitOn)
{
}
