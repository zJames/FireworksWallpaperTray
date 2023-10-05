//------------------------------------------------------------------------------
// Process.h
//------------------------------------------------------------------------------
#ifndef PROCESS_H
#define PROCESS_H

#include "../Function/Function.h"
#include "../Pointers/WeakPtr.h"

#include <functional>
#include <list>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
class ProcessList;
class Process;


typedef std::function<Process*()> Replacer;


//------------------------------------------------------------------------------
// Class Declarations
//------------------------------------------------------------------------------
class Process : public Weakable
{
public:
	enum Status
	{
		ST_NORMAL,
		ST_PAUSED,
		ST_DELETE,
		ST_REPLACE,

		ST_NUM,
	};

	Process();
	virtual ~Process();

	void pause(bool);
	void queueDelete();
	void exec();
	ProcessList* parent() const;
	Status status() const;
	void replaceProcess(Process*);
	void queueReplace(const Replacer& replacer);

protected:
	virtual void update();

private:
	friend class ProcessList;

	ProcessList*	mParent;
	Status			mStatus;
	Replacer		mReplacer;
};


class ProcessList: public Process, protected std::list<Process*>
{
public:
	using std::list<Process*>::empty;

	virtual ~ProcessList();

	using list<Process*>::size;
	
	void add(Process*);
	void remove(Process*);
	void deleteAll();
	void queueDeleteAll();

protected:
	/*override*/ void update();
};



////////////////////////////////////////////////////////////////////////////////
#endif