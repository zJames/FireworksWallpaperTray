//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Convert.h"

#include "ConstTextIterator.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Convert
//------------------------------------------------------------------------------
std::vector<std::string> convertToStringArray(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::vector<std::string> values;

	while (iter.get())
	{
		values.push_back(iter.get());
		++iter;
	}

	return values;
}

std::vector<RefString> convertToRefStringArray(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::vector<RefString> values;

	while (iter.get())
	{
		values.push_back(RefString::copy(iter.get()));
		++iter;
	}

	return values;
}

std::vector<int> convertToIntArray(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::vector<int> values;

	while (iter.get())
	{
		values.push_back(convertToInt(iter.get()));
		++iter;
	}

	return values;
}

std::list<std::string> convertToStringList(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::list<std::string> values;

	while (iter.get())
	{
		values.push_back(iter.get());
		++iter;
	}

	return values;
}

std::list<RefString> convertToRefStringList(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::list<RefString> values;

	while (iter.get())
	{
		values.push_back(RefString::copy(iter.get()));
		++iter;
	}

	return values;
}

std::list<int> convertToIntList(const char* input, const char* seperator)
{
	ConstTextIterator iter(const_cast<char*>(input), seperator);
	std::list<int> values;

	while (iter.get())
	{
		values.push_back(convertToInt(iter.get()));
		++iter;
	}

	return values;
}

int convertToInt(const char* input)
{
	if (input == NULL)
	{
		return 0;
	}

	return atoi(input);
}

float convertToFloat(const char* input)
{
	if (input == NULL)
	{
		return 0;
	}

	return (float)atof(input);
}

bool convertToBool(const char* input)
{
	if (input == NULL)
	{
		return false;
	}

	bool ret = false;

	if (_stricmp(input, "true") == 0)
		ret = true;
	else if (convertToInt(input) != 0)
		ret = true;

	return ret;
}

Rect convertToRect(const char* input)
{
	std::vector<int> values = convertToIntArray((*input == '{') ? input + 1 : input, ",");

	Rect ret;

	if (values.size() == 4)
	{
		ret.left = values[0];
		ret.top = values[1];
		ret.right = values[2];
		ret.bottom = values[3];
	}

	return ret;
}

Vector2i convertToVector2i(const char* input)
{
	std::vector<int> values = convertToIntArray((*input == '{') ? input + 1 : input, ",");

	Vector2i ret(0, 0);

	if (values.size() >= 2)
	{
		ret.x = values[0];
		ret.y = values[1];
	}

	return ret;
}

RefStringW convertToW(const RefString& input)
{
	RefStringW ret(input.dataSize());

	for (size_t i = 0; i < input.dataSize(); ++i)
	{
		ret.ptr()[i] = input.const_ptr()[i];
	}

	return ret;
}

int convertToInt(const char* input, const StringMap* stringMap, size_t stringMapCount)
{
	if (input == NULL)
	{
		return 0;
	}

	//Check if input is an integer
	const int test = convertToInt(input);

	if (test != 0)
	{
		return test;
	}

	//Check if input is an enumeration string
	for (size_t i = 0; i < stringMapCount; ++i)
	{
		if (strcmp(stringMap[i].text, input) == 0)
		{
			return stringMap[i].value;
		}
	}

	return 0;
}