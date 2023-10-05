//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "TextIteratorBase.h"
#include "../Text/RefString.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ConstTextIterator
//------------------------------------------------------------------------------
class ConstTextIterator : public TextIteratorBase
{
public:
	ConstTextIterator(ConstTextType text, char splitOn);
	ConstTextIterator(ConstTextType text, const char* splitOn);
	ConstTextIterator(ConstTextType text, const SplitArray& splitOn);

private:
	void setup();

	RefString mTextCopy;
};