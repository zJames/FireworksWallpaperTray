//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Pointers/Ptr.h"
#include "../Text/RefString.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	TextIteratorBase
//------------------------------------------------------------------------------
class TextIteratorBase
{
public:
	typedef Ptr<char> TextType;
	typedef Ptr<const char> ConstTextType;
	typedef std::vector< RefString > SplitArray;

	TextIteratorBase(char splitOn);
	TextIteratorBase(const char* splitOn);
	TextIteratorBase(const SplitArray& splitOn);
	TextIteratorBase(TextType text, char splitOn);
	TextIteratorBase(TextType text, const char* splitOn);
	TextIteratorBase(TextType text, const SplitArray& splitOn);

	const char* get() const;

	TextIteratorBase& operator++();

protected:
	bool isSplit(TextType test) const;
	TextType findNext() const;

	void findFirst();

	SplitArray mSplits;
	TextType mText;
	TextType mCurrent;
	TextType mNext;
	char mBackup;
};
