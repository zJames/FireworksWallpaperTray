//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "WindowsInc.h"

#include "../Pointers/NonNull.h"
#include "../Pointers/Ptr.h"

#include <tchar.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	MemoryDLL
//------------------------------------------------------------------------------
class MemoryDLL
{
public:
	typedef void *HMEMORYMODULE;
	typedef void *HMEMORYRSRC;

	MemoryDLL(NonNull<const char> dll, void* data = NULL);
	~MemoryDLL();

	FARPROC getProcAddress(const char* procName) const;
	HMEMORYRSRC getResourceHandle(LPCSTR name, LPCSTR type);
	HMEMORYRSRC getResourceHandleEx(LPCSTR name, LPCSTR type, WORD language);
	DWORD getResourceSize(HMEMORYRSRC resource);
	LPVOID getResource(HMEMORYRSRC resource);
	size_t getString(UINT index, TCHAR* buffer, size_t bufferSize);
	size_t getStringEx(UINT id, TCHAR* buffer, size_t bufferSize, WORD language);

private:
	void load(void* customData);
	HMEMORYMODULE loadInternal(void* customData);
	void unload();

	NonNull<const char> mData;
	HMEMORYMODULE mHandle;
};
