//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ConstTextIterator.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ConstTextIterator
//------------------------------------------------------------------------------
ConstTextIterator::ConstTextIterator(ConstTextType text, char splitOn)
	:
	TextIteratorBase(splitOn),
	mTextCopy(text.get())
{
	setup();
}

ConstTextIterator::ConstTextIterator(ConstTextType text, const char* splitOn)
	:
	TextIteratorBase(splitOn),
	mTextCopy(text.get())
{
	setup();
}

ConstTextIterator::ConstTextIterator(ConstTextType text, const SplitArray& splitOn)
	:
	TextIteratorBase(splitOn),
	mTextCopy(text.get())
{
	setup();
}

void ConstTextIterator::setup()
{
	mCurrent = mTextCopy.ptr();
	findFirst();
}