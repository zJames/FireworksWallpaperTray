//------------------------------------------------------------------------------
//  Copyright : (c) 2011
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "BitSet.h"

#include <assert.h>
#include <list>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	BlockAllocator
//------------------------------------------------------------------------------
template<class T, int kNumPerBlock = 32, bool kAutoCullEmpty = true>
class BlockAllocator
{
public:
	static const int kObjectsPerBlock = kNumPerBlock;
	static const int kSizePerObj = sizeof(T);
	static const bool kCullsEmpty = kAutoCullEmpty;

private:
	typedef BitSet<kNumPerBlock> CheckBits;
	typedef char ObjBuff[kSizePerObj];

	struct Block
	{
		size_t indexOf(const char* obj) const
		{
			const char* start = (const char*)objects[0];
			return (obj - start) / kSizePerObj;
		}

		bool contains(const char* obj) const
		{
			const void* start = objects;
			const void* end = objects + (kSizePerObj * kObjectsPerBlock);

			return (obj >= start && obj < end);
		}

		CheckBits allocs;
		ObjBuff objects[kObjectsPerBlock];
	};

	typedef std::list<Block> BlockList;

public:
	BlockAllocator()
	{
	}

	~BlockAllocator()
	{
#ifdef _DEBUG
		cullEmpty();
		//		assert(mBlocks.empty()); //check for leakage
#endif
	}

	T* allocate()
	{
		T* newObj = NULL;

		for (typename BlockList::iterator i = mBlocks.begin(); i != mBlocks.end(); ++i)
		{
			const int firstFree = i->allocs.firstNotSet();

			if (firstFree >= 0)
			{
				i->allocs.set(firstFree);
				newObj = (T*)(i->objects[firstFree]);

				break;
			}
		}

		if (newObj == NULL)
		{
			mBlocks.push_back(Block());
			return allocate();
		}

		return newObj;
	}

	template<class A>
	T* construct(const A& a)
	{
		T* newObj = allocate();

		new (newObj) T(a);

		return newObj;
	}

	template<class A, class B>
	T* construct(const A& a, const B& b)
	{
		T* newObj = allocate();

		new (newObj) T(a, b);

		return newObj;
	}

	template<class A, class B, class C>
	T* construct(const A& a, const B& b, const C& c)
	{
		T* newObj = allocate();

		new (newObj) T(a, b, c);

		return newObj;
	}

	template<class A, class B, class C, class D>
	T* construct(const A& a, const B& b, const C& c, const D& d)
	{
		T* newObj = allocate();

		new (newObj) T(a, b, c, d);

		return newObj;
	}

	void free(T* object)
	{
		for (typename BlockList::iterator i = mBlocks.begin(); i != mBlocks.end(); ++i)
		{
			if (i->contains((char*)object))
			{
				const size_t idx = i->indexOf((char*)object);
				//assert(i->allocs.test(idx)); //check for double free
				i->allocs.set(idx, false);
				object->~T();
			}
		}

		if (kCullsEmpty)
		{
			//TODO: This breaks in x64 release mode
			cullEmpty();
		}
	}

	void cullEmpty()
	{
		for (typename BlockList::iterator i = mBlocks.begin(); i != mBlocks.end();)
		{
			if (i->allocs.none())
				i = mBlocks.erase(i);
			else
				++i;
		}
	}

private:
	BlockList mBlocks;
};