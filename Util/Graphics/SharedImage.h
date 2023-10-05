//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Bitmap.h"
#include "../Core/SharedT.h"
#include "../Text/RefString.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Image
//------------------------------------------------------------------------------
class Image: public Bitmap
{
public:
	class Key
	{
	public:
		Key();
		Key(const char*);
		Key(int, HINSTANCE);
		Key(RefString);

		Key& operator=(const Key&);

		bool operator==(const Key&) const;
		bool operator>(const Key&) const;
		bool operator<(const Key&) const;

		bool usesResource() const { return mUsesResource; }
		const RefString& file() const { return mFile; }
		const int resource() const { return mResource; }
		const HINSTANCE hInst() const { return mHInst; }

	protected:
		int mResource;
		HINSTANCE mHInst;
		bool mUsesResource;
		RefString mFile;
	};

	Image();
	Image(const Key&);
};


//------------------------------------------------------------------------------
//	SharedImage
//------------------------------------------------------------------------------
typedef SharedT<Image, Image::Key> SharedImage;