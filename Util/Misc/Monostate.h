//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Pointers/ScopedPtr.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Monostate
//------------------------------------------------------------------------------
namespace MonostateDetail
{
	template<class T, class Name>
	class MonoDetail
	{
	public:
		class Reference
		{
		public:
			typedef MonoDetail<T, Name> Parent;

			Reference()
			{
				if (Parent::sRefCount == 0)
					Parent::sMonoVal.set(new T());

				++Parent::sRefCount;
			}

			Reference(T* newVal)
			{
				assert(Parent::sRefCount == 0);

				Parent::sMonoVal.set(newVal);

				++Parent::sRefCount;
			}

			~Reference()
			{
				--Parent::sRefCount;

				if (Parent::sRefCount == 0)
					Parent::sMonoVal.set(NULL);
			}
		};

		class FirstReference : public Reference
		{
		public:
			FirstReference(T* newVal)
				:
				Reference(newVal)
			{
			}

			FirstReference()
				:
				Reference(new T())
			{
			}
		};

		T* get() { return sMonoVal.get(); }
		T* operator -> () { assert(sMonoVal.get() != NULL); return sMonoVal.get(); }
		T& operator * () { assert(sMonoVal.get() != NULL); return *sMonoVal.get(); }

	private:
		friend class Reference;
		friend class FirstReference;

		static void addRef()
		{
			++sRefCount;
		}

		static void removeRef()
		{
			--sRefCount;

			if (sRefCount == 0)
			{
				sMonoVal.reset(NULL);
			}

			assert(sRefCount >= 0);
		}

		static int sRefCount;
		static ScopedPtr<T> sMonoVal;
	};
}

#define Monostate(Type, name) \
	class name: public MonostateDetail::MonoDetail<Type, name> {}

template<class T, class Name> ScopedPtr<T> MonostateDetail::MonoDetail<T, Name>::sMonoVal;
template<class T, class Name> int MonostateDetail::MonoDetail<T, Name>::sRefCount;
