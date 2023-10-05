//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "MemoryDLL.h"

#include "MemoryDLL_Util.h"
#include "../Core/Debug.h"

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Defines
//------------------------------------------------------------------------------
#define DEFAULT_LANGUAGE MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)


//------------------------------------------------------------------------------
//	MemoryDLL
//------------------------------------------------------------------------------
MemoryDLL::MemoryDLL(NonNull<const char> dll, void* data)
	:
	mData(dll),
	mHandle(NULL)
{
	load(data);
}

MemoryDLL::~MemoryDLL()
{
	unload();
}

FARPROC MemoryDLL::getProcAddress(const char* procName) const
{
	unsigned char *codeBase = ((PMEMORYMODULE)mHandle)->codeBase;
	DWORD idx = -1;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)mHandle, IMAGE_DIRECTORY_ENTRY_EXPORT);
	
	if (directory->Size == 0)
	{
		// no export table found
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)(codeBase + directory->VirtualAddress);
	
	if (exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0)
	{
		// DLL doesn't export anything
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	if (HIWORD(procName) == 0)
	{
		// load function by ordinal value
		if (LOWORD(procName) < exports->Base)
		{
			SetLastError(ERROR_PROC_NOT_FOUND);
			return NULL;
		}

		idx = LOWORD(procName) - exports->Base;
	}
	else
	{
		// search function name in list of exported names
		DWORD* nameRef = (DWORD*)(codeBase + exports->AddressOfNames);
		WORD* ordinal = (WORD*)(codeBase + exports->AddressOfNameOrdinals);
		BOOL found = FALSE;
		
		for (DWORD i = 0; i < exports->NumberOfNames; i++, ++nameRef, ++ordinal)
		{
			if (_stricmp(procName, (const char *)(codeBase + (*nameRef))) == 0)
			{
				idx = *ordinal;
				found = TRUE;
				break;
			}
		}

		if (!found)
		{
			// exported symbol not found
			SetLastError(ERROR_PROC_NOT_FOUND);
			return NULL;
		}
	}

	if (idx > exports->NumberOfFunctions)
	{
		// name <-> ordinal number don't match
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	// AddressOfFunctions contains the RVAs to the "real" functions
	return (FARPROC)(codeBase + (*(DWORD*)(codeBase + exports->AddressOfFunctions + (idx * sizeof(DWORD)))));
}

HMEMORYRSRC MemoryDLL::getResourceHandle(LPCSTR name, LPCSTR type)
{
	return getResourceHandleEx(name, type, DEFAULT_LANGUAGE);
}

HMEMORYRSRC MemoryDLL::getResourceHandleEx(LPCSTR name, LPCSTR type, WORD language)
{
	unsigned char *codeBase = ((PMEMORYMODULE)mHandle)->codeBase;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)mHandle, IMAGE_DIRECTORY_ENTRY_RESOURCE);
	PIMAGE_RESOURCE_DIRECTORY rootResources;
	PIMAGE_RESOURCE_DIRECTORY nameResources;
	PIMAGE_RESOURCE_DIRECTORY typeResources;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY foundType;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY foundName;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY foundLanguage;
	
	if (directory->Size == 0)
	{
		// no resource table found
		SetLastError(ERROR_RESOURCE_DATA_NOT_FOUND);
		return NULL;
	}

	if (language == DEFAULT_LANGUAGE)
	{
		// use language from current thread
		language = LANGIDFROMLCID(GetThreadLocale());
	}

	// resources are stored as three-level tree
	// - first node is the type
	// - second node is the name
	// - third node is the language
	rootResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress);
	foundType = MemorySearchResourceEntry(rootResources, rootResources, type);
	
	if (foundType == NULL)
	{
		SetLastError(ERROR_RESOURCE_TYPE_NOT_FOUND);
		return NULL;
	}

	typeResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress + (foundType->OffsetToData & 0x7fffffff));
	foundName = MemorySearchResourceEntry(rootResources, typeResources, name);
	
	if (foundName == NULL)
	{
		SetLastError(ERROR_RESOURCE_NAME_NOT_FOUND);
		return NULL;
	}

	nameResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress + (foundName->OffsetToData & 0x7fffffff));
	foundLanguage = MemorySearchResourceEntry(rootResources, nameResources, (LPCTSTR)(uintptr_t)language);
	
	if (foundLanguage == NULL)
	{
		// requested language not found, use first available
		if (nameResources->NumberOfIdEntries == 0)
		{
			SetLastError(ERROR_RESOURCE_LANG_NOT_FOUND);
			return NULL;
		}

		foundLanguage = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(nameResources + 1);
	}

	return (codeBase + directory->VirtualAddress + (foundLanguage->OffsetToData & 0x7fffffff));
}

DWORD MemoryDLL::getResourceSize(HMEMORYRSRC resource)
{
	PIMAGE_RESOURCE_DATA_ENTRY entry = (PIMAGE_RESOURCE_DATA_ENTRY)resource;

	if (entry == NULL)
	{
		return 0;
	}

	return entry->Size;
}

LPVOID MemoryDLL::getResource(HMEMORYRSRC resource)
{
	unsigned char *codeBase = ((PMEMORYMODULE)mHandle)->codeBase;
	PIMAGE_RESOURCE_DATA_ENTRY entry = (PIMAGE_RESOURCE_DATA_ENTRY)resource;

	if (entry == NULL)
	{
		return NULL;
	}

	return codeBase + entry->OffsetToData;
}

size_t MemoryDLL::getString(UINT index, TCHAR* buffer, size_t bufferSize)
{
	return getStringEx(index, buffer, bufferSize, DEFAULT_LANGUAGE);
}

size_t MemoryDLL::getStringEx(UINT id, TCHAR* buffer, size_t bufferSize, WORD language)
{
	if (bufferSize == 0)
	{
		return 0;
	}

	HMEMORYRSRC resource = getResourceHandleEx(MAKEINTRESOURCE((id >> 4) + 1), RT_STRING, language);
	
	if (resource == NULL)
	{
		buffer[0] = 0;
		return 0;
	}

	PIMAGE_RESOURCE_DIR_STRING_U data = (PIMAGE_RESOURCE_DIR_STRING_U)getResource(resource);
	id = id & 0x0f;
	
	while (id--)
	{
		data = (PIMAGE_RESOURCE_DIR_STRING_U)(((char *)data) + (data->Length + 1) * sizeof(WCHAR));
	}
	if (data->Length == 0)
	{
		SetLastError(ERROR_RESOURCE_NAME_NOT_FOUND);
		buffer[0] = 0;
		return 0;
	}

	size_t size = data->Length;
	
	if (size >= bufferSize)
	{
		size = bufferSize;
	}
	else
	{
		buffer[size] = 0;
	}

#if defined(UNICODE)
	wcsncpy(buffer, data->NameString, size);
#else
	size_t convertedChars;
	wcstombs_s(&convertedChars, buffer, bufferSize, data->NameString, size);
#endif

	return size;
}

void MemoryDLL::load(void* customData)
{
	mHandle = loadInternal(customData);

	if (mHandle != NULL)
	{
		const int result = MemoryCallEntryPoint(mHandle);

		if (result != 0)
		{
			LogDebugArgs("Entry point for dll(%08X) returned result: %d", mData.get(), result);
		}
	}
}

MemoryDLL::HMEMORYMODULE MemoryDLL::loadInternal(void* customData)
{
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)mData.get();

	if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
	{
		SetLastError(ERROR_BAD_EXE_FORMAT);
		return NULL;
	}

	PIMAGE_NT_HEADERS old_header = (PIMAGE_NT_HEADERS)&((const unsigned char *)(mData.get()))[dos_header->e_lfanew];

	if (old_header->Signature != IMAGE_NT_SIGNATURE)
	{
		SetLastError(ERROR_BAD_EXE_FORMAT);
		return NULL;
	}

	if (old_header->FileHeader.Machine != HEADER_IMAGE)
	{
		SetLastError(ERROR_BAD_EXE_FORMAT);
		return NULL;
	}

	if (old_header->OptionalHeader.SectionAlignment & 1)
	{
		// Only support section alignments that are a multiple of 2
		SetLastError(ERROR_BAD_EXE_FORMAT);
		return NULL;
	}

	// reserve memory for image of library
	// XXX: is it correct to commit the complete memory region at once?
	//      calling DllEntry raises an exception if we don't...
	unsigned char* code =
		(unsigned char *)VirtualAlloc(
			(LPVOID)(old_header->OptionalHeader.ImageBase),
			old_header->OptionalHeader.SizeOfImage,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE);

	if (code == NULL)
	{
		// try to allocate memory at arbitrary position
		code =
			(unsigned char *)VirtualAlloc(NULL,
				old_header->OptionalHeader.SizeOfImage,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE);

		if (code == NULL)
		{
			SetLastError(ERROR_OUTOFMEMORY);
			return NULL;
		}
	}

	PMEMORYMODULE result = (PMEMORYMODULE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MEMORYMODULE));

	if (result == NULL)
	{
		VirtualFree(code, 0, MEM_RELEASE);
		SetLastError(ERROR_OUTOFMEMORY);
		return NULL;
	}

	result->codeBase = code;
	result->isDLL = (old_header->FileHeader.Characteristics & IMAGE_FILE_DLL) != 0;
	result->loadLibrary = _LoadLibrary;
	result->getProcAddress = _GetProcAddress;
	result->freeLibrary = _FreeLibrary;
	result->userdata = customData;

	SYSTEM_INFO sysInfo;

	GetNativeSystemInfo(&sysInfo);

	result->pageSize = sysInfo.dwPageSize;

	// commit memory for headers
	unsigned char* headers =
		(unsigned char *)VirtualAlloc(code,
			old_header->OptionalHeader.SizeOfHeaders,
			MEM_COMMIT,
			PAGE_READWRITE);

	// copy PE header to code
	memcpy(headers, dos_header, old_header->OptionalHeader.SizeOfHeaders);
	result->headers = (PIMAGE_NT_HEADERS)&((const unsigned char *)(headers))[dos_header->e_lfanew];

	// update position
	result->headers->OptionalHeader.ImageBase = (uintptr_t)code;

	size_t locationDelta = 0;

	// copy sections from DLL file block to new memory location
	if (!CopySections((const unsigned char *)mData.get(), old_header, result))
	{
		goto error;
	}

	// adjust base address of imported data
	locationDelta = (size_t)(code - old_header->OptionalHeader.ImageBase);

	if (locationDelta != 0)
	{
		result->isRelocated = PerformBaseRelocation(result, locationDelta);
	}
	else
	{
		result->isRelocated = TRUE;
	}

	// load required dlls and adjust function table of imports
	if (!BuildImportTable(result))
	{
		goto error;
	}

	// mark memory pages depending on section headers and release
	// sections that are marked as "discardable"
	if (!FinalizeSections(result))
	{
		goto error;
	}

	// TLS callbacks are executed BEFORE the main loading
	if (!ExecuteTLS(result))
	{
		goto error;
	}

	// get entry point of loaded library
	if (result->headers->OptionalHeader.AddressOfEntryPoint != 0)
	{
		if (result->isDLL)
		{
			DllEntryProc entry = (DllEntryProc)(code + result->headers->OptionalHeader.AddressOfEntryPoint);
			// notify library about attaching to process
			BOOL successfull = (*entry)((HINSTANCE)code, DLL_PROCESS_ATTACH, 0);
			
			if (!successfull)
			{
				SetLastError(ERROR_DLL_INIT_FAILED);
				goto error;
			}

			result->initialized = TRUE;
		}
		else
		{
			result->exeEntry = (ExeEntryProc)(code + result->headers->OptionalHeader.AddressOfEntryPoint);
		}
	}
	else
	{
		result->exeEntry = NULL;
	}

	return (HMEMORYMODULE)result;

error:
	// cleanup
	unload();
	return NULL;
}

void MemoryDLL::unload()
{
	PMEMORYMODULE module = (PMEMORYMODULE)mHandle;

	if (module == NULL)
	{
		return;
	}
	
	if (module->initialized)
	{
		// notify library about detaching from process
		DllEntryProc entry = (DllEntryProc)(module->codeBase + module->headers->OptionalHeader.AddressOfEntryPoint);
		(*entry)((HINSTANCE)module->codeBase, DLL_PROCESS_DETACH, 0);
	}

	if (module->modules != NULL)
	{
		// free previously opened libraries
		for (int i = 0; i < module->numModules; i++)
		{
			if (module->modules[i] != NULL)
			{
				module->freeLibrary(module->modules[i], module->userdata);
			}
		}

		free(module->modules);
	}

	if (module->codeBase != NULL)
	{
		// release memory of library
		VirtualFree(module->codeBase, 0, MEM_RELEASE);
	}

	HeapFree(GetProcessHeap(), 0, module);
}