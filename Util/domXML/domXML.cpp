//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "domXML.h"

#include "Parse.h"
#include "../Core/Debug.h"
#include "../Core/File.h"

#include <stdio.h>
#include <string>
////////////////////////////////////////////////////////////////////////////////


namespace DomXML
{

//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
bool validForAttrib(const char c)
{
	switch (c)
	{
	case '=':
	case '"':
		return true;

	default:
		return false;
	}
}


//------------------------------------------------------------------------------
//	Node
//------------------------------------------------------------------------------
Node::Node()
	:
	mPtr(0)
{
}

Node::Node(const SharedType& data, const char* start)
	:
	mData(data),
	mPtr(start)
{
	this->mName = findOffsets(start, mOffsets);
}

Node::Node(const Node& other)
	:
	mPtr(other.mPtr)
{
	mName = other.mName;
	mData = other.mData;
	mOffsets = other.mOffsets;
}

void Node::load(const SharedType& data, const char* start)
{
	mData = data;
	mPtr = start;
	mName.clear();
	findOffsets(start, mOffsets);
}

const RefString& Node::name() const
{
	return mName;
}

RefString Node::attribute(const char* attrib) const
{
	if (!isValid())
	{
		return RefString();
	}

	const char* cur = mPtr;
	const int attribLen = (int)strlen(attrib);

	bool inString = false;

	while ((inString || *cur != '>') && *cur)
	{
		cur = eatWhite(cur + 1);

		if (!inString && comp(attrib, cur, false))
		{
			if (!validForAttrib(*eatWhite(cur + attribLen)))
			{
				continue;
			}

			return readAttrib(cur + attribLen);
		}

		if (*cur == '"' && *(cur - 1) != '\\')
		{
			inString ^= true;
		}
	}

	return RefString();
}

RefString Node::insideText() const
{
	if (!isValid())
	{
		return RefString();
	}

	const char* cur = mPtr;

	bool inString = false;

	while ((inString || *cur != '>') && *cur)
	{
		++cur;

		if (*cur == '"' && *(cur - 1) != '\\')
		{
			inString ^= true;
		}
	}

	++cur;
	const char* start = cur;

	while (*cur && *cur != '<' && *(cur + 1) != '/' && !comp(cur + 2, mName))
	{
		++cur;
	}

	return RefString::copy(start, cur - start);
}

bool Node::isValid() const
{
	return mPtr != NULL && *mPtr != 0;
}

Node& Node::operator =(const Node& other)
{
	mPtr = other.mPtr;
	mData = other.mData;
	mName = other.mName;
	mOffsets = other.mOffsets;

	return *this;
}

Node Node::operator[](size_t n) const
{
	return getNode(n);
}

int Node::size() const
{
	return (int)mOffsets.size();
}

Node Node::getNode(size_t index) const
{
	if (!isValid() || index >= mOffsets.size())
	{
		return Node();
	}

	return Node(mData, mPtr + mOffsets[index]);
}

Node Node::getNode(const RefString& name) const
{
	if (!isValid())
	{
		return Node();
	}

	for (int i = 0; i < size(); ++i)
	{
		const Node check = getNode(i);

		if (check.name() == name)
		{
			return check;
		}
	}

	return Node();
}


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------
Node getFirstNode(const SharedArray<const char>& file)
{
	const char kBOM[] = { (char)0xEF, (char)0xBB, (char)0xBF };

	const char* start = file.get();
	bool inString = false;

	if (strlen(start) > 3 && start[0] == kBOM[0] && start[1] == kBOM[1] && start[2] == kBOM[2])
	{
		start = eatWhite(start + 3);
	}

	if (comp(start, "<?xml", false))
	{
		while ((inString || *start != '>') && *start != 0)
		{
			if (*start == '\"')
			{
				inString ^= true;
			}

			++start;
		}

		if (*start == '>')
		{
			++start;
		}
	}

	start = (char*)eatWhite(start);

	return Node(file, start);
}

Node getFirstNode(const char* fileName)
{
	if (fileName == NULL)
	{
		return Node();
	}

	File file(fileName, File::eOption_Read_Text);

	if (!file.isValid())
	{
		return Node();
	}

	const size_t length = file.size();

	if (length == 0)
	{
		LogDebugArgs("domXML: Tried to load invalid file (%s)\n", fileName);
		return Node();
	}

	SharedArray<const char> data(new char[length]);

	file.read(const_cast<char*>(data.get()), length);

	return getFirstNode(data);
}

} //end namespace DomXML
