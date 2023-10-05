//------------------------------------------------------------------------------
// Process.cpp
//------------------------------------------------------------------------------

#include "Process.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// ProcessList Implementation
//------------------------------------------------------------------------------
ProcessList::~ProcessList()
{
	deleteAll();
}


void ProcessList::add(Process* p)
{
	if (!p)
		return;

	if (p->mParent)
	{
		p->mParent->remove(p);
	}

	p->mParent = this;

	push_back(p);
}


void ProcessList::remove(Process* p)
{
	for (iterator i = begin(); i != end(); ++i)
	{
		if (*i == p)
		{
			erase(i);
			return;
		}
	}
}


void ProcessList::update()
{
	for (iterator i = begin(); i != end();)
	{
		(*i)->exec();

		switch ((*i)->status())
		{
		case ST_REPLACE:
		{
			std::function<Process*()> func = (*i)->mReplacer;

			delete *i;
			i = erase(i);

			if (func)
			{
				add(func());
			}
		}
		break;

		case ST_DELETE:
		{
			delete *i;
			i = erase(i);
		}
		break;

		default:
			++i;
			break;
		}
	}
}


void ProcessList::deleteAll()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		delete *i;
	}

	clear();
}

void ProcessList::queueDeleteAll()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		(*i)->queueDelete();
	}
}


//------------------------------------------------------------------------------
// Process Implementation
//------------------------------------------------------------------------------
Process::Process()
	:mStatus(ST_NORMAL)
	, mParent(NULL)
{
}


Process::~Process()
{
}


void Process::queueDelete()
{
	mStatus = ST_DELETE;
}


void Process::exec()
{
	if (mStatus == ST_NORMAL)
	{
		update();
	}
}


ProcessList* Process::parent() const
{
	return mParent;
}


void Process::pause(bool p)
{
	if (mStatus == ST_DELETE)
	{
		return;
	}

	mStatus = p ? ST_PAUSED : ST_NORMAL;
}


void Process::update()
{

}


Process::Status Process::status() const
{
	return mStatus;
}


void Process::replaceProcess(Process* p)
{
	queueDelete();

	if (mParent)
	{
		mParent->add(p);
	}
	else
	{
		delete p;
	}
}


void Process::queueReplace(const Replacer& replacer)
{
	mReplacer = replacer;
	mStatus = ST_REPLACE;
}
