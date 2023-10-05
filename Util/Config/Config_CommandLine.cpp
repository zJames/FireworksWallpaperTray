//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Config_CommandLine.h"

#include "../Misc/ConstTextIterator.h"
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	Local Functions
	//------------------------------------------------------------------------------
	bool hasPrefix(const char* input, const char* prefix)
	{
		if (input == NULL)
			return false;

		if (prefix == NULL)
			return true;

		while (*input && *prefix)
		{
			if (*input != *prefix)
				return false;

			++input;
			++prefix;
		}

		return true;
	}

	RefString getLine(ConstTextIterator& iter, const char* seperator)
	{
		RefString line = "";

		{
			ConstTextIterator partial(iter.get(), seperator);

			++partial;

			if (partial.get() == NULL)
				return "";

			line += partial.get();
		}

		if (line.const_ptr()[0] != '"')
			return line;

		do
		{
			++iter;

			if (iter.get())
			{
				line += " ";
				line += iter.get();
			}
			else
			{
				break;
			}
		} while (line.const_ptr()[line.length() - 1] != '"');

		return line;
	}

	RefString makeCopy(const char* text)
	{
		return RefString::copy(text);
	}

	//------------------------------------------------------------------------------
	//	CommandLine
	//------------------------------------------------------------------------------
	CommandLine::CommandLine(const char* argv[], int argc, const char* _prefix, const char* _seperator)
		:
		prefix(_prefix),
		seperator(_seperator)
	{
		const size_t prefixLen = _prefix == NULL ? 0 : strlen(_prefix);

		for (int i = 1; i < argc; ++i)
		{
			const char* arg = argv[i];

			if (!hasPrefix(arg, _prefix))
				continue;

			ConstTextIterator iter(arg, _seperator);

			RefString name = makeCopy(iter.get() + prefixLen);
			++iter;

			mStrings[name] = makeCopy(iter.get() ? iter.get() : "");
		}
	}

	CommandLine::CommandLine(const char* cmdLine, const char* _prefix, const char* _seperator)
		:
		prefix(_prefix),
		seperator(_seperator)
	{
		const size_t prefixLen = _prefix == NULL ? 0 : strlen(_prefix);

		ConstTextIterator iter(cmdLine, " ");

		while (iter.get())
		{
			if (hasPrefix(iter.get(), _prefix))
			{
				ConstTextIterator nameIter(iter.get(), _seperator);
				RefString name = makeCopy(nameIter.get() + prefixLen);

				mStrings[name] = getLine(iter, _seperator);
			}

			++iter;
		}
	}

	const char* CommandLine::getRawOption(const char* name) const
	{
		StringMap::const_iterator obj = mStrings.find(name);

		if (obj == mStrings.end())
		{
			return NULL;
		}

		return obj->second;
	}

} //end namespace Config