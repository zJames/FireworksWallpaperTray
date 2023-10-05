//------------------------------------------------------------------------------
//  Copyright : (c) 2013
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "TextIteratorBase.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	TextIterator
//------------------------------------------------------------------------------
class TextIterator : public TextIteratorBase
{
public:
	TextIterator(TextType text, char splitOn);
	TextIterator(TextType text, const char* splitOn);
	TextIterator(TextType text, const SplitArray& splitOn);
};