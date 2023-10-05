//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


#include <map>


//------------------------------------------------------------------------------
//	SharedT
//------------------------------------------------------------------------------
template<class TypeArg, class KeyArg>
class SharedT
{
public:
	typedef SharedT<TypeArg, KeyArg> This;
	typedef TypeArg Type;
	typedef KeyArg Key;

	SharedT();
	explicit SharedT(const KeyArg&);
	SharedT(const This& other);

	void acquire(const Key&);
	void clear();

	inline Type* get() { return mVal; }
	inline This& operator =(const This& other) { acquire(other.key()); return *this; }
	inline Type& operator *() { return *mVal; }
	inline Type* operator->() { return mVal; }

	inline const Type* get() const { return mVal; }
	inline const Key& key() const { return mKey; }
	inline const Type& operator *() const { return *mVal; }
	inline const Type* operator->() const { return mVal; }

private:
	struct Entry
	{
		Entry(const Key& key): refCount(0), obj(key) {}

		int refCount;
		Type obj;
	};

	typedef std::map<Key, Entry*> ShareMap;

	Key mKey;
	Type* mVal;

	static ShareMap sShares;
};


#define SHAREDT_ARGS template<class TypeArg, class KeyArg>
#define SHAREDT_TYPE SharedT<TypeArg, KeyArg>


//------------------------------------------------------------------------------
//	Implementation
//------------------------------------------------------------------------------
SHAREDT_ARGS SHAREDT_TYPE::SharedT(): mVal(0) {}
SHAREDT_ARGS SHAREDT_TYPE::SharedT(const KeyArg & key): mVal(0) { acquire(key); }
SHAREDT_ARGS SHAREDT_TYPE::SharedT(const SHAREDT_TYPE& other): mVal(0) { acquire(other.key()); }


SHAREDT_ARGS void SHAREDT_TYPE::acquire(const Key& key)
{
	if (mVal && mKey == key)
		return;

	clear();

	typename ShareMap::iterator i = sShares.find(key);

	if (i == sShares.end())
	{
		sShares[key] = new Entry(key);

		i = sShares.find(key);
	}

	++i->second->refCount;

	mKey = key;
	mVal = &i->second->obj;
}


SHAREDT_ARGS void SHAREDT_TYPE::clear()
{
	if (mVal == 0)
		return;

	const typename ShareMap::iterator i = sShares.find(mKey);

	if (i == sShares.end())
		return;

	--i->second->refCount;

	if (i->second->refCount == 0)
	{
		delete i->second;
		sShares.erase(i);
	}

	mVal = 0;
}


SHAREDT_ARGS typename SHAREDT_TYPE::ShareMap SHAREDT_TYPE::sShares;