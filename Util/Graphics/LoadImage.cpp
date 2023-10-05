//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "LoadImage.h"

#include "loadILBM.h"
#include "LoadPCX.h"
#include "LoadPNG.h"
#include "../Core/Debug.h"
#include "../Misc/TextManip.h"
#include "../Text/RefString.h"

//#define TEST_FORCE_IMAGE
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	LoadImage
//------------------------------------------------------------------------------
namespace Graphics
{

	void LoadImage(const char* txtr, Bitmap& image)
	{
		const RefString& ext = getFileExtension(txtr);

		if (ext.empty())
		{
			LogDebugArgs("Invalid file name passed to load image: %s\n", txtr);
			return;
		}
		else if (_strcmpi("bmp", ext) == 0)
		{
			image.loadBMPFile(txtr);
		}
		else if (_strcmpi("png", ext) == 0)
		{
			LoadPNG(txtr, image);
		}
		else if (_strcmpi("pcx", ext) == 0)
		{
			loadPCX(txtr, image);
		}
		else if (_strcmpi("lbm", ext) == 0)
		{
			loadILBM(txtr, image);
		}
	}


	void LoadImage(const char* txtr, CycleBitmap& cycleImage)
	{
		const RefString& ext = getFileExtension(txtr);

		if (ext.empty())
		{
			LogDebugArgs("Invalid file name passed to load image: %s\n", txtr);
			return;
		}
		else if (_strcmpi("lbm", ext.c_str()) == 0)
		{
			loadILBM(txtr, cycleImage);
		}
		else
		{
			LoadImage(txtr, cycleImage.image, Bitmap::F_INDEXED);
		}
	}


	void LoadImage(const char* txtr, Bitmap& image, Bitmap::Format format)
	{
		LoadImage(txtr, image);
		ForceFormat(image, format);
	}


	void LoadImage(const void* data, size_t dataSize, Bitmap& image)
	{
		const unsigned char* charData = (const unsigned char*)data;

		if (charData[0] == 'B' && charData[1] == 'M')
		{
			image.loadData((const char*)data);
		}
		else if (charData[0] == 137 && charData[1] == 'P' && charData[2] == 'N' && charData[3] == 'G')
		{
			if (!LoadPNG(charData, dataSize, image))
				LogDebug("LoadImage: ERROR: Could not load PNG from data\n");
		}
	}


	void LoadImage(const void* data, size_t dataSize, Bitmap& image, Bitmap::Format format)
	{
		LoadImage(data, dataSize, image);
		ForceFormat(image, format);
	}


	void LoadImage(LPCSTR resourceId, HINSTANCE hInst, Bitmap& image)
	{
		HRSRC myResource = ::FindResource(hInst, resourceId, RT_BITMAP);

		if (myResource == NULL)
		{
			LogDebugArgs("Couldn't find resource: %d\n", resourceId);
			return;
		}

		HGLOBAL myResourceData = ::LoadResource(hInst, myResource);
		void* pMyBinaryData = ::LockResource(myResourceData);

		LoadImage(pMyBinaryData, ::SizeofResource(hInst, myResource), image);
	}


	void LoadImage(LPCSTR resourceId, HINSTANCE hInst, Bitmap& image, Bitmap::Format format)
	{
		LoadImage(resourceId, hInst, image);
		ForceFormat(image, format);
	}


	void ForceFormat(Bitmap& image, Bitmap::Format format)
	{
		if (image.format() == format)
			return;

		Bitmap temp;

		temp.setSize(image.width(), image.height(), format);
		image.drawAll(temp);
		image.swap(temp);

#if defined(_DEBUG) & defined(TEST_FORCE_IMAGE)
		static int testIndex = 0;

		char filename[512] = {0};

		sprintf_s(filename, "test_%04d.bmp", testIndex++);

		image.saveBMPFile(filename);
#endif
	}

} //end namespace Graphics