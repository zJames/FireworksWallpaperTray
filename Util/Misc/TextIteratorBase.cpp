//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "TextIteratorBase.h"
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
//	TextIteratorBase
//------------------------------------------------------------------------------
TextIteratorBase::TextIteratorBase(char splitOn)
{
	char charToStr[2] = {splitOn, 0};
	mSplits.push_back(charToStr);
}

TextIteratorBase::TextIteratorBase(const char* splitOn)
{
	mSplits.push_back(splitOn);
}

TextIteratorBase::TextIteratorBase(const SplitArray& splitOn)
	:
	mSplits(splitOn)
{
}

TextIteratorBase::TextIteratorBase(TextType text, char splitOn)
	:
	mText(text),
	mCurrent(text)
{
	char charToStr[2] = {splitOn, 0};
	mSplits.push_back(charToStr);

	findFirst();
}

TextIteratorBase::TextIteratorBase(TextType text, const char* splitOn)
	:
	mText(text),
	mCurrent(text)
{
	mSplits.push_back(splitOn);

	findFirst();
}

TextIteratorBase::TextIteratorBase(TextType text, const SplitArray& splitOn)
	:
	mText(text),
	mCurrent(text),
	mSplits(splitOn)
{
	findFirst();
}

const char* TextIteratorBase::get() const
{
	return mCurrent.get();
}

TextIteratorBase& TextIteratorBase::operator++ ()
{
	mCurrent = mNext;

	if (mNext.get() != NULL)
	{
		*mNext = mBackup;
		++mCurrent;
	}

	mNext = findNext();

	if (mNext.get() == NULL)
	{
		if (isSplit(mCurrent))
		{
			mCurrent = NULL;
		}

		return *this;
	}

	mBackup = *mNext;
	*mNext = 0;

	if (isSplit(mCurrent))
	{
		++(*this);
	}

	return *this;
}

bool TextIteratorBase::isSplit(TextType test) const
{
	for (size_t i = 0; i < mSplits.size(); ++i)
	{
		if (comp(test.get(), mSplits[i].c_str()))
		{
			return true;
		}
	}

	return false;
}

TextIteratorBase::TextType TextIteratorBase::findNext() const
{
	if (mCurrent == NULL)
		return TextType();

	TextType check = mCurrent;

	while (*check != 0)
	{
		if (isSplit(check))
		{
			return check;
		}

		++check;
	}

	return TextType();
}

void TextIteratorBase::findFirst()
{
	mNext = findNext();

	if (mNext.get() != NULL)
	{
		mBackup = *mNext;
		*mNext = 0;
	}
}
