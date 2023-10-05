//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "BitSet.h"
#include "ObjBuff.h"
#include "../Pointers/ScopedPtr.h"
#include "../Pointers/Ptr.h"

#include <list>
////////////////////////////////////////////////////////////////////////////////


template<class T, int kBlockSizeLog2 = 3>
class BlockArray
{
public:
	static const int kBlocksPerGroup = 1 << kBlockSizeLog2;
	typedef StructBuf<T> Block;
	typedef BitSet<kBlocksPerGroup> BlockBits;

	struct BlockGroupStruct
	{
		~BlockGroupStruct()
		{
			clear();
		}

		void clear()
		{
			for (int i = 0; i < kBlocksPerGroup; ++i)
			{
				if (bits[i])
				{
					blocks[i].erase();
				}
			}

			bits.clear();
		}

		BlockBits bits;
		Block blocks[kBlocksPerGroup];
	};

	typedef BlockGroupStruct* BlockGroup;
	typedef std::list<BlockGroup> BlockList;

	class Iterator
	{
	public:
		Iterator& operator ++()
		{
			if (mBlockIter != mBlockList->end())
			{
				incSubIndex();
			}

			return *this;
		}

		Iterator& operator--()
		{
			if (mBlockIter != mBlockList->begin() || mSubIndex > 0)
			{
				decSubIndex();
			}

			return *this;
		}

		Iterator& operator =(const Iterator& other)
		{
			mBlockList = other.mBlockList;
			mBlockIter = other.mBlockIter;
			mSubIndex = other.mSubIndex;

			return *this;
		}

		bool operator != (const Iterator& other) const
		{
			return
				mBlockList != other.mBlockList
				|| mBlockIter != other.mBlockIter
				|| mSubIndex != other.mSubIndex;
		}

		bool operator == (const Iterator& other) const
		{
			return
				mBlockIter == other.mBlockIter
				&& mBlockList == other.mBlockList
				&& mSubIndex == other.mSubIndex;
		}

		T& operator *() const
		{
			return *block().get();
		}

		Block& operator ->() const
		{
			return block();
		}

		int subIndex() const
		{
			return mSubIndex;
		}

	private:
		friend class BlockArray;

		Iterator(BlockList const* blockList)
			:
			mBlockList(blockList),
			mSubIndex(0)
		{
			mBlockIter = blockList->begin();

			if (blockList->empty())
			{
				return;
			}

			while (!(*mBlockIter)->bits[mSubIndex])
			{
				++mSubIndex;
			}
		}

		Iterator(BlockList const* blockList, typename BlockList::const_iterator blockItr, int subIndex = 0)
			:
			mBlockList(blockList),
			mBlockIter(blockItr),
			mSubIndex(subIndex)
		{
		}

		Block& block() const
		{
			return (*mBlockIter)->blocks[mSubIndex];
		}

		BlockBits& bits() const
		{
			return (*mBlockIter)->bits;
		}

		void incSubIndexInner()
		{
			if (mBlockIter == mBlockList->end())
			{
				return;
			}

			++mSubIndex;

			if (mSubIndex == kBlocksPerGroup)
			{
				++mBlockIter;
				mSubIndex = 0;
			}
		}

		void incSubIndex()
		{
			do
			{
				incSubIndexInner();
			} while (mBlockIter != mBlockList->end() && !(*mBlockIter)->bits[mSubIndex]);
		}

		void decSubIndexInner()
		{
			if (mSubIndex > 0)
			{
				--mSubIndex;
				return;
			}

			if (mBlockIter == mBlockList->begin())
			{
				return;
			}

			--mBlockIter;
			mSubIndex = kBlocksPerGroup - 1;
		}

		void decSubIndex()
		{
			do
			{
				decSubIndexInner();
			} while ((mBlockIter != mBlockList->begin() || mSubIndex > 0) && !(*mBlockIter)->bits[mSubIndex]);
		}

		Ptr<const BlockList> mBlockList;
		typename BlockList::const_iterator mBlockIter;
		int mSubIndex;
	};

	typedef Iterator iterator;

	BlockArray()
	{
	}

	~BlockArray()
	{
		freeMemory();
	}

	void construct()
	{
		Iterator it = alloc();
		it.block.construct();
		it.bits().set(it.subIndex());
	}

	template<typename... Arguments>
	void construct(Arguments... parameters)
	{
		Iterator it = alloc();
		it.block().construct(parameters...);
		it.bits().set(it.subIndex());
	}

	T& front()
	{
		return *begin();
	}

	T& back()
	{
		Iterator it = end();

		--it;

		return *it;
	}

	Iterator begin() const
	{
		return Iterator(&mBlocks);
	}

	Iterator end() const
	{
		return Iterator(&mBlocks, mBlocks.cend());
	}

	Iterator erase(Iterator it)
	{
		if (it == end())
		{
			return it;
		}

		const Iterator oldIt = it;
		++it;

		oldIt.bits().set(oldIt.subIndex(), false);

		if (oldIt.bits().none())
		{
			delete (*oldIt.mBlockIter);
			mBlocks.erase(oldIt.mBlockIter);
		}
		else
		{
			oldIt.block().erase();
		}

		return it;
	}

	bool empty() const { return size() == 0; }

	void pop_front()
	{
		erase(begin());
	}

	void pop_back()
	{
		erase(--end());
	}

	void clear()
	{
		for (typename BlockList::iterator it = mBlocks.begin(); it != mBlocks.end(); ++it)
		{
			(*it)->clear();
		}
	}

	size_t size() const
	{
		size_t count = 0;

		for (typename BlockList::const_iterator i = mBlocks.begin(); i != mBlocks.end(); ++i)
		{
			count += (*i)->bits.count();
		}

		return count;
	}

	void freeMemory()
	{
		clear();

		mBlocks.clear();
	}

private:
	Iterator alloc()
	{
		if (!mBlocks.empty())
		{
			BlockGroup& block = mBlocks.back();
			const int last = block->bits.lastSet() + 1;

			if (block->bits.count() < kBlocksPerGroup && last < kBlocksPerGroup)
			{
				return Iterator(&mBlocks, (--mBlocks.end()), last);
			}
		}

		mBlocks.push_back(BlockGroup(new BlockGroupStruct()));

		return Iterator(&mBlocks, --mBlocks.end());
	}

	BlockList mBlocks;
};