//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "XmlWrite.h"

#include "../Core/File.h"
////////////////////////////////////////////////////////////////////////////////


namespace XmlWrite
{
	//------------------------------------------------------------------------------
	//	Local Data
	//------------------------------------------------------------------------------
	const char kXmlHeader[] = "<?xml version=\"1.0\" encoding=\"UTF - 8\" standalone=\"no\" ?>\n";

	//------------------------------------------------------------------------------
	//	Local Functions
	//------------------------------------------------------------------------------
	void write(File& file, const Node* node, int depth)
	{
		if (node == NULL)
		{
			return;
		}

		if (depth > 0)
		{
			fprintf(file, "%*s", depth, " ");
			fprintf(file, "%*s", depth, " ");
		}

		fprintf(file, "<%s ", node->name.c_str());

		for (Node::AttributeMap::const_iterator i = node->attributes.begin(); i != node->attributes.end(); ++i)
		{
			fprintf(file, "%s=\"%s\" ", i->first.c_str(), i->second.c_str());
		}

		if (node->subNodes.empty())
		{
			fprintf(file, "/>\n");
		}
		else
		{
			fprintf(file, ">\n");

			for (Node::NodeList::const_iterator i = node->subNodes.begin(); i != node->subNodes.end(); ++i)
			{
				const SharedPtr<Node>& subNode = *i;
				write(file, subNode.get(), depth + 1);
			}

			fprintf(file, "</%s>\n", node->name.c_str());
		}
	}

	//------------------------------------------------------------------------------
	//	XmlWrite
	//------------------------------------------------------------------------------
	bool write(const char* fileName, const Node& root)
	{
		File file(fileName, File::eOption_Write_Text);

		if (!file.isValid())
		{
			return false;
		}

		fprintf(file, kXmlHeader);
		write(file, &root, 0); //write root node

		return true;
	}
} //end namespace XmlWrite