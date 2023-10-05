//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Parse.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Global Parsing Functions
//------------------------------------------------------------------------------


bool isWhite(char c)
{
	switch (c)
	{
	case '\t':
	case '\n':
	case ' ':
	case '\r':
		return true;

	default:
		return false;
	}
}


bool isWhiteOrNull(const char c)
{
	return (c == 0) || isWhite(c);
}


bool comp(const char* first, const char* second, bool checkEnd)
{
	if (first == NULL || second == NULL)
	{
		return false;
	}

	while (*first && *second)
	{
		if (*first != *second)
			return false;

		++first;
		++second;
	}

	if (!checkEnd)
	{
		return true;
	}

	return isWhiteOrNull(*first) && isWhiteOrNull(*second);
}


const char* eatWhite(const char* start)
{
	while (start != NULL && isWhite(*start) && *start)
	{
		++start;
	}

	return start;
}


size_t tokenLen(const char* start)
{
	size_t size = 0;

	while (!isWhite(*start) && *start)
	{
		++size;
		++start;
	}

	return size;
}


RefString getToken(const char*& start)
{
	start = eatWhite(start);

	const size_t len = tokenLen(start);
	RefString ret = RefString::copy(start, len);

	start += len;

	return ret;
}


const char* find(const char* start, const char seek)
{
	if (start == NULL)
	{
		return NULL;
	}

	while (*start != seek && *start)
	{
		++start;
	}

	return start;
}


const char* parseOver(const char* start)
{
	const char* cur = find(start, '<');

	if (*cur == 0)
	{
		return cur;
	}

	cur = eatWhite(cur + 1);

	if (*cur == 0)
	{
		return cur;
	}

	RefString name;

	while (!isWhiteOrNull(*cur) && *cur != '>')
	{
		name += *cur;
		++cur;
	}

	bool inString = false;

	while ((inString || *cur != '>') && *cur)
	{
		++cur;

		if (*cur == '"' && *(cur - 1) != '\\')
		{
			inString ^= true;
		}
	}

	if (*(cur - 1) == '/' && *cur == '>')
	{
		return cur + 1;
	}

	while (*cur)
	{
		cur = find(cur, '<');

		if (*cur == 0)
		{
			return cur;
		}

		if (*(cur + 1) == '/' && comp(cur + 2, name, false))
		{
			return find(cur, '>');
		}

		cur = parseOver(cur);
	}

	return cur;
}


const char* skipAttrib(const char* start)
{
	const char* cur = start;
	bool inString = false;
	bool equalSeen = false;

	while (isWhiteOrNull(*cur) || inString || !equalSeen)
	{
		switch (*cur)
		{
		case '=':
			if (!inString)
			{
				equalSeen = true;
				cur = eatWhite(cur + 1);
			}
			break;

		case '"':
			if (*(cur - 1) != '\\')
			{
				inString ^= true;
			}

			++cur;
			break;

		default:
			++cur;
			break;
		}
	}

	return cur;
}


RefString findOffsets(const char* const start, std::vector<int>& offsets)
{
	if (start == NULL)
	{
		return "";
	}

	const char* cur = eatWhite(find(start, '<'));

	RefString name;

	//Skip over the '<'
	if (*cur)
	{
		++cur;
	}

	while (!isWhite(*cur) && *cur != '>')
	{
		name += *cur;
		++cur;
	}

	bool inString = false;

	while ((inString || *cur != '>') && *cur)
	{
		++cur;

		if (*cur == '"' && *(cur - 1) != '\\')
		{
			inString ^= true;
		}
	}

	bool endFound = false;

	cur = find(cur, '>');

	if (cur == NULL || *cur == 0)
	{
		return name;
	}

	if (*cur == '>' && *(cur - 1) == '/')
	{
		return name;
	}

	//Skip over the '>'
	cur = eatWhite(cur + 1);

	while (!endFound && *cur)
	{
		if (*cur == '<')
		{
			if (*(cur + 1) == '/' && comp(cur + 2, name, false))
			{
				endFound = true;
			}
			else
			{
				offsets.push_back((int)(cur - start));
				cur = parseOver(cur);

				if (*cur == '>')
				{
					cur = eatWhite(cur + 1);
				}
			}
		}
		else
		{
			cur = eatWhite(cur + 1);
		}
	}

	return name;
}


RefString readAttrib(const char* start)
{
	RefString ret;

	const char* cur = eatWhite(start);

	if (*cur == '=')
	{
		cur = eatWhite(cur + 1);
	}

	bool inString = false;

	if (*cur == '"')
	{
		++cur;
		inString = true;
	}
	else
	{
		return RefString();
	}

	//Empty string case
	if (*cur == '"')
	{
		return RefString();
	}

	const char* strStart = cur;

	while (*cur)
	{
		++cur;

		if (*cur == '"' && *(cur - 1) != '\\' && inString)
		{
			break;
		}
	}

	return RefString::copy(strStart, cur - strStart);
}
