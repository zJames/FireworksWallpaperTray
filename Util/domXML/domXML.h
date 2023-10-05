//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


#include "domXmlDefines.h"
#include "../Pointers/SharedPtr.h"

namespace DomXML
{

class Node
{
public:
	typedef SharedArray<const char> SharedType;

	Node();
	Node(const SharedType&data, const char* start);
	Node(const Node& other);

	void load(const SharedType& data, const char* start);

	const RefString& name() const;
	RefString attribute(const char* attribute) const;
	RefString insideText() const;
	bool isValid() const;

	int size() const;
	Node getNode(size_t index) const;
	Node getNode(const RefString& name) const;

	Node& operator =(const Node& other);

	Node operator[](size_t n) const;

private:
	const char* mPtr;
	SharedType mData;
	RefString mName;
	std::vector<int> mOffsets;
};


Node getFirstNode(const SharedArray<const char>& file);
Node getFirstNode(const char* fileName);

} //end namespace DomXML
