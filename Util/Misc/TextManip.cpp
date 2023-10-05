//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "TextManip.h"

#include <string.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	TextManip
//------------------------------------------------------------------------------
RefString getFileExtension(const char* fileName)
{
	size_t idx = strlen(fileName);

	while (idx > 0 && fileName[idx] != '.')
	{
		--idx;
	}

	if (idx == 0)
		return RefString();

	return fileName + idx + 1;
}

size_t findLastSlash(const char* fileName)
{
	for (size_t i = strlen(fileName) - 1; i > 0; --i)
	{
		if (fileName[i] == '/' || fileName[i] == '\\')
		{
			return i;
		}
	}

	return 0;
}

size_t findLastDot(const char* fileName)
{
	size_t len = strlen(fileName);

	for (size_t i = len - 1; i > 0; --i)
	{
		if (fileName[i] == '/' || fileName[i] == '\\')
		{
			return len;
		}

		if (fileName[i] == '.')
		{
			return i;
		}
	}

	return len;
}

RefString plainName(const char* fileName)
{
	size_t lastSlash = findLastSlash(fileName);
	size_t lastDot = findLastDot(fileName);

	return RefString::copy(fileName + (lastSlash > 0 ? lastSlash + 1 : 0), lastDot - lastSlash);
}

RefString directoryPath(const char* filePath)
{
	const size_t pos = findLastSlash(filePath);

	if (pos == 0)
	{
		return RefString();
	}

	RefString ret(pos + 1);

	memcpy_s(ret.ptr(), ret.dataSize(), filePath, pos);
	ret[pos] = 0;

	return ret;
}