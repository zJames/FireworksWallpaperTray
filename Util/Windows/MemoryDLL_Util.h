//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Defines
//------------------------------------------------------------------------------
#define DEBUG_OUTPUT

#ifdef _WIN64
#define HEADER_IMAGE IMAGE_FILE_MACHINE_AMD64
#else
#define HEADER_IMAGE IMAGE_FILE_MACHINE_I386
#endif

#ifndef IMAGE_SIZEOF_BASE_RELOCATION
// Vista SDKs no longer define IMAGE_SIZEOF_BASE_RELOCATION!?
#define IMAGE_SIZEOF_BASE_RELOCATION (sizeof(IMAGE_BASE_RELOCATION))
#endif

#define GET_HEADER_DICTIONARY(module, idx)  &(module)->headers->OptionalHeader.DataDirectory[idx]
#define ALIGN_DOWN(address, alignment)      (LPVOID)((uintptr_t)(address) & ~(uintptr_t(alignment) - 1))



//------------------------------------------------------------------------------
//	Types
//------------------------------------------------------------------------------
typedef void *HMEMORYRSRC;
typedef void *HCUSTOMMODULE;
typedef void *HMEMORYMODULE;

typedef BOOL(WINAPI *DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
typedef int (WINAPI *ExeEntryProc)(void);
typedef HCUSTOMMODULE(*CustomLoadLibraryFunc)(LPCSTR, void *);
typedef FARPROC(*CustomGetProcAddressFunc)(HCUSTOMMODULE, LPCSTR, void *);
typedef void(*CustomFreeLibraryFunc)(HCUSTOMMODULE, void *);


typedef struct
{
	PIMAGE_NT_HEADERS headers;
	unsigned char *codeBase;
	HCUSTOMMODULE *modules;
	int numModules;
	BOOL initialized;
	BOOL isDLL;
	BOOL isRelocated;
	CustomLoadLibraryFunc loadLibrary;
	CustomGetProcAddressFunc getProcAddress;
	CustomFreeLibraryFunc freeLibrary;
	void *userdata;
	ExeEntryProc exeEntry;
	DWORD pageSize;
} MEMORYMODULE, *PMEMORYMODULE;

typedef struct
{
	LPVOID address;
	LPVOID alignedAddress;
	DWORD size;
	DWORD characteristics;
	BOOL last;
} SECTIONFINALIZEDATA, *PSECTIONFINALIZEDATA;


//------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------
#ifdef DEBUG_OUTPUT
void OutputLastError(const char *msg);
#endif

HCUSTOMMODULE _LoadLibrary(LPCSTR filename, void *userdata);
FARPROC _GetProcAddress(HCUSTOMMODULE module, LPCSTR name, void *userdata);
void _FreeLibrary(HCUSTOMMODULE module, void *userdata);
BOOL CopySections(const unsigned char *data, PIMAGE_NT_HEADERS old_headers, PMEMORYMODULE module);
BOOL PerformBaseRelocation(PMEMORYMODULE module, SIZE_T delta);
BOOL BuildImportTable(PMEMORYMODULE module);
BOOL ExecuteTLS(PMEMORYMODULE module);
BOOL FinalizeSection(PMEMORYMODULE module, PSECTIONFINALIZEDATA sectionData);
BOOL FinalizeSections(PMEMORYMODULE module);
DWORD GetRealSectionSize(PMEMORYMODULE module, PIMAGE_SECTION_HEADER section);
int MemoryCallEntryPoint(HMEMORYMODULE mod);
PIMAGE_RESOURCE_DIRECTORY_ENTRY MemorySearchResourceEntry(
	void *root,
	PIMAGE_RESOURCE_DIRECTORY resources,
	LPCTSTR key);
