//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "SharedImage.h"

#include "LoadImage.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Image::Key
//------------------------------------------------------------------------------
Image::Key::Key() : mUsesResource(false) {}

Image::Key::Key(int resource, HINSTANCE handle) : mUsesResource(true), mResource(resource), mHInst(handle) {}

Image::Key::Key(RefString fileName) : mFile(fileName), mUsesResource(false) {}

Image::Key::Key(const char* fileName) : mFile(RefString::copy(fileName)), mUsesResource(false) {}


Image::Key& Image::Key::operator =(const Key& other)
{
	mUsesResource = other.mUsesResource;
	mResource = other.mResource;
	mFile = other.mFile;

	return *this;
}


bool Image::Key::operator ==(const Key& other) const
{
	if (mUsesResource != other.mUsesResource)
	{
		return false;
	}

	if (mUsesResource)
	{
		return mResource == other.mResource;
	}

	return mFile == other.mFile;
}


bool Image::Key::operator >(const Key& other) const
{
	if (mUsesResource && !other.mUsesResource)
	{
		return true;
	}

	if (!mUsesResource && other.mUsesResource)
	{
		return false;
	}

	if (mUsesResource)
	{
		return mResource > other.mResource;
	}

	return mFile > other.mFile;
}


bool Image::Key::operator <(const Key& other) const
{
	if (*this == other)
	{
		return false;
	}

	return !(*this > other);
}


//------------------------------------------------------------------------------
//	Image
//------------------------------------------------------------------------------
Image::Image() {}

Image::Image(const Key& key)
{
	if (key.usesResource())
	{
		loadResource(MAKEINTRESOURCE(key.resource()), key.hInst());
	}
	else
	{
		Graphics::LoadImage(key.file(), *this);
	}
}