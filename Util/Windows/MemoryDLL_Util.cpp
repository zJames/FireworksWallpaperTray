//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "MemoryDLL_Util.h"

#include "../Core/Debug.h"

#include <stdio.h>
#include <tchar.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Data
//------------------------------------------------------------------------------
// Protection flags for memory pages (Executable, Readable, Writeable)
static int kProtectionFlags[2][2][2] =
{
	{
		// not executable
		{PAGE_NOACCESS, PAGE_WRITECOPY},
		{PAGE_READONLY, PAGE_READWRITE},
	},
	{
		// executable
		{PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY},
		{PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE},
	},
};


//------------------------------------------------------------------------------
//	MemoryDLL_Util
//------------------------------------------------------------------------------
#ifdef DEBUG_OUTPUT
void OutputLastError(const char *msg)
{
	LPVOID tmp;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&tmp,
		0,
		NULL);

	size_t tmpSize = strlen(msg) + strlen((const char*)tmp) + 3;
	char* tmpmsg = (char *)LocalAlloc(LPTR, tmpSize);

	sprintf_s(tmpmsg, tmpSize, "%s: %s", msg, (const char*)tmp);
	LogDebug(tmpmsg);
	LocalFree(tmpmsg);
	LocalFree(tmp);
}
#endif

HCUSTOMMODULE _LoadLibrary(LPCSTR filename, void *userdata)
{
	HMODULE result = LoadLibraryA(filename);
	if (result == NULL)
	{
		return NULL;
	}

	return (HCUSTOMMODULE)result;
}

FARPROC _GetProcAddress(HCUSTOMMODULE module, LPCSTR name, void *userdata)
{
	return (FARPROC)GetProcAddress((HMODULE)module, name);
}

void _FreeLibrary(HCUSTOMMODULE module, void *userdata)
{
	FreeLibrary((HMODULE)module);
}

BOOL CopySections(const unsigned char *data, PIMAGE_NT_HEADERS old_headers, PMEMORYMODULE module)
{
	unsigned char *codeBase = module->codeBase;
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);

	for (int i = 0; i < module->headers->FileHeader.NumberOfSections; i++, section++)
	{
		unsigned char *dest = NULL;

		if (section->SizeOfRawData == 0)
		{
			// section doesn't contain data in the dll itself, but may define
			// uninitialized data
			int size = old_headers->OptionalHeader.SectionAlignment;

			if (size > 0)
			{
				dest = (unsigned char *)VirtualAlloc(codeBase + section->VirtualAddress,
					size,
					MEM_COMMIT,
					PAGE_READWRITE);
				if (dest == NULL)
				{
					return FALSE;
				}

				// Always use position from file to support alignments smaller
				// than page size.
				dest = codeBase + section->VirtualAddress;
				section->Misc.PhysicalAddress = (DWORD)(uintptr_t)dest;
				memset(dest, 0, size);
			}

			// section is empty
			continue;
		}

		// commit memory block and copy data from dll
		dest =
			(unsigned char *)VirtualAlloc(
				codeBase + section->VirtualAddress,
				section->SizeOfRawData,
				MEM_COMMIT,
				PAGE_READWRITE);

		if (dest == NULL)
		{
			return FALSE;
		}

		// Always use position from file to support alignments smaller
		// than page size.
		dest = codeBase + section->VirtualAddress;
		memcpy(dest, data + section->PointerToRawData, section->SizeOfRawData);
		section->Misc.PhysicalAddress = (DWORD)(uintptr_t)dest;
	}

	return TRUE;
}

BOOL PerformBaseRelocation(PMEMORYMODULE module, SIZE_T delta)
{
	unsigned char *codeBase = module->codeBase;

	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_BASERELOC);

	if (directory->Size == 0)
	{
		return (delta == 0);
	}

	PIMAGE_BASE_RELOCATION relocation = (PIMAGE_BASE_RELOCATION)(codeBase + directory->VirtualAddress);

	while (relocation != NULL && relocation->VirtualAddress > 0)
	{
		unsigned char *dest = codeBase + relocation->VirtualAddress;
		unsigned short *relInfo = (unsigned short *)((unsigned char *)relocation + IMAGE_SIZEOF_BASE_RELOCATION);

		for (DWORD i = 0; i < ((relocation->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2); i++, relInfo++)
		{
			DWORD *patchAddrHL;
#ifdef _WIN64
			ULONGLONG *patchAddr64;
#endif
			int type, offset;

			// the upper 4 bits define the type of relocation
			type = *relInfo >> 12;
			// the lower 12 bits define the offset
			offset = *relInfo & 0xfff;

			switch (type)
			{
			case IMAGE_REL_BASED_ABSOLUTE:
				// skip relocation
				break;

			case IMAGE_REL_BASED_HIGHLOW:
				// change complete 32 bit address
				patchAddrHL = (DWORD *)(dest + offset);
				*patchAddrHL += (DWORD)delta;
				break;

#ifdef _WIN64
			case IMAGE_REL_BASED_DIR64:
				patchAddr64 = (ULONGLONG *)(dest + offset);
				*patchAddr64 += (ULONGLONG)delta;
				break;
#endif

			default:
				//printf("Unknown relocation: %d\n", type);
				break;
			}
		}

		// advance to next relocation block
		relocation = (PIMAGE_BASE_RELOCATION)(((char *)relocation) + relocation->SizeOfBlock);
	}

	return TRUE;
}

BOOL BuildImportTable(PMEMORYMODULE module)
{
	unsigned char *codeBase = module->codeBase;

	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_IMPORT);

	if (directory->Size == 0)
	{
		return TRUE;
	}

	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(codeBase + directory->VirtualAddress);
	BOOL result = TRUE;

	for (; !IsBadReadPtr(importDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR)) && importDesc->Name; importDesc++)
	{
		HCUSTOMMODULE handle = module->loadLibrary((LPCSTR)(codeBase + importDesc->Name), module->userdata);

		if (handle == NULL)
		{
			SetLastError(ERROR_MOD_NOT_FOUND);
			result = FALSE;
			break;
		}

		HCUSTOMMODULE* tmp = (HCUSTOMMODULE *)realloc(module->modules, (module->numModules + 1)*(sizeof(HCUSTOMMODULE)));

		if (tmp == NULL)
		{
			module->freeLibrary(handle, module->userdata);
			SetLastError(ERROR_OUTOFMEMORY);
			result = FALSE;
			break;
		}

		module->modules = tmp;
		module->modules[module->numModules++] = handle;

		uintptr_t *thunkRef;
		FARPROC *funcRef;

		if (importDesc->OriginalFirstThunk)
		{
			thunkRef = (uintptr_t *)(codeBase + importDesc->OriginalFirstThunk);
			funcRef = (FARPROC *)(codeBase + importDesc->FirstThunk);
		}
		else
		{
			// no hint table
			thunkRef = (uintptr_t *)(codeBase + importDesc->FirstThunk);
			funcRef = (FARPROC *)(codeBase + importDesc->FirstThunk);
		}

		for (; *thunkRef; thunkRef++, funcRef++)
		{
			if (IMAGE_SNAP_BY_ORDINAL(*thunkRef))
			{
				*funcRef = module->getProcAddress(handle, (LPCSTR)IMAGE_ORDINAL(*thunkRef), module->userdata);
			}
			else
			{
				PIMAGE_IMPORT_BY_NAME thunkData = (PIMAGE_IMPORT_BY_NAME)(codeBase + (*thunkRef));
				*funcRef = module->getProcAddress(handle, (LPCSTR)&thunkData->Name, module->userdata);
			}

			if (*funcRef == 0)
			{
				result = FALSE;
				break;
			}
		}

		if (!result)
		{
			module->freeLibrary(handle, module->userdata);
			SetLastError(ERROR_PROC_NOT_FOUND);
			break;
		}
	}

	return result;
}

BOOL ExecuteTLS(PMEMORYMODULE module)
{
	unsigned char *codeBase = module->codeBase;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_TLS);

	if (directory->VirtualAddress == 0)
	{
		return TRUE;
	}

	PIMAGE_TLS_DIRECTORY tls = (PIMAGE_TLS_DIRECTORY)(codeBase + directory->VirtualAddress);
	PIMAGE_TLS_CALLBACK* callback = (PIMAGE_TLS_CALLBACK *)tls->AddressOfCallBacks;
	
	if (callback)
	{
		while (*callback)
		{
			(*callback)((LPVOID)codeBase, DLL_PROCESS_ATTACH, NULL);
			callback++;
		}
	}

	return TRUE;
}

BOOL FinalizeSection(PMEMORYMODULE module, PSECTIONFINALIZEDATA sectionData)
{
	if (sectionData->size == 0)
	{
		return TRUE;
	}

	if (sectionData->characteristics & IMAGE_SCN_MEM_DISCARDABLE)
	{
		// section is not needed any more and can safely be freed
		if (sectionData->address == sectionData->alignedAddress &&
			(sectionData->last
				|| module->headers->OptionalHeader.SectionAlignment == module->pageSize
				|| (sectionData->size % module->pageSize) == 0)
			)
		{
			// Only allowed to decommit whole pages
			VirtualFree(sectionData->address, sectionData->size, MEM_DECOMMIT);
		}
		return TRUE;
	}

	// determine protection flags based on characteristics
	BOOL executable = (sectionData->characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
	BOOL readable = (sectionData->characteristics & IMAGE_SCN_MEM_READ) != 0;
	BOOL writeable = (sectionData->characteristics & IMAGE_SCN_MEM_WRITE) != 0;
	DWORD protect = kProtectionFlags[executable][readable][writeable];

	if (sectionData->characteristics & IMAGE_SCN_MEM_NOT_CACHED)
	{
		protect |= PAGE_NOCACHE;
	}

	DWORD oldProtect;

	// change memory access flags
	if (VirtualProtect(sectionData->address, sectionData->size, protect, &oldProtect) == 0)
	{
#ifdef DEBUG_OUTPUT
		OutputLastError("Error protecting memory page");
#endif
		return FALSE;
	}

	return TRUE;
}

BOOL FinalizeSections(PMEMORYMODULE module)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);
#ifdef _WIN64
	uintptr_t imageOffset = (module->headers->OptionalHeader.ImageBase & 0xffffffff00000000);
#else
#define imageOffset 0
#endif
	SECTIONFINALIZEDATA sectionData;

	sectionData.address = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
	sectionData.alignedAddress = ALIGN_DOWN(sectionData.address, module->pageSize);
	sectionData.size = GetRealSectionSize(module, section);
	sectionData.characteristics = section->Characteristics;
	sectionData.last = FALSE;

	++section;

	// loop through all sections and change access flags
	for (int i = 1; i<module->headers->FileHeader.NumberOfSections; ++i, ++section)
	{
		LPVOID sectionAddress = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
		LPVOID alignedAddress = ALIGN_DOWN(sectionAddress, module->pageSize);
		DWORD sectionSize = GetRealSectionSize(module, section);
		// Combine access flags of all sections that share a page
		// TODO(fancycode): We currently share flags of a trailing large section
		//   with the page of a first small section. This should be optimized.
		if (sectionData.alignedAddress == alignedAddress || (uintptr_t)sectionData.address + sectionData.size >(uintptr_t) alignedAddress)
		{
			// Section shares page with previous
			if ((section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0 || (sectionData.characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0)
			{
				sectionData.characteristics = (sectionData.characteristics | section->Characteristics) & ~IMAGE_SCN_MEM_DISCARDABLE;
			}
			else
			{
				sectionData.characteristics |= section->Characteristics;
			}
			sectionData.size = DWORD((((uintptr_t)sectionAddress) + sectionSize) - (uintptr_t)sectionData.address);
			continue;
		}

		if (!FinalizeSection(module, &sectionData))
		{
			return FALSE;
		}

		sectionData.address = sectionAddress;
		sectionData.alignedAddress = alignedAddress;
		sectionData.size = sectionSize;
		sectionData.characteristics = section->Characteristics;
	}

	sectionData.last = TRUE;

	if (!FinalizeSection(module, &sectionData))
	{
		return FALSE;
	}
#ifndef _WIN64
#undef imageOffset
#endif
	return TRUE;
}

DWORD GetRealSectionSize(PMEMORYMODULE module, PIMAGE_SECTION_HEADER section)
{
	DWORD size = section->SizeOfRawData;

	if (size == 0)
	{
		if (section->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA)
		{
			size = module->headers->OptionalHeader.SizeOfInitializedData;
		}
		else if (section->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA)
		{
			size = module->headers->OptionalHeader.SizeOfUninitializedData;
		}
	}

	return size;
}

int MemoryCallEntryPoint(HMEMORYMODULE mod)
{
	PMEMORYMODULE module = (PMEMORYMODULE)mod;

	if (module == NULL || module->isDLL || module->exeEntry == NULL || !module->isRelocated)
	{
		return -1;
	}

	return module->exeEntry();
}

PIMAGE_RESOURCE_DIRECTORY_ENTRY MemorySearchResourceEntry(
	void *root,
	PIMAGE_RESOURCE_DIRECTORY resources,
	LPCTSTR key)
{
	PIMAGE_RESOURCE_DIRECTORY_ENTRY entries = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(resources + 1);
	PIMAGE_RESOURCE_DIRECTORY_ENTRY result = NULL;
	DWORD start;
	DWORD end;
	DWORD middle;

	if (!IS_INTRESOURCE(key) && key[0] == TEXT('#'))
	{
		// special case: resource id given as string
		TCHAR *endpos = NULL;
		long int tmpkey = (WORD)_tcstol((TCHAR *)&key[1], &endpos, 10);
		if (tmpkey <= 0xffff && lstrlen(endpos) == 0)
		{
			key = MAKEINTRESOURCE(tmpkey);
		}
	}

	// entries are stored as ordered list of named entries,
	// followed by an ordered list of id entries - we can do
	// a binary search to find faster...
	if (IS_INTRESOURCE(key))
	{
		WORD check = (WORD)(uintptr_t)key;
		start = resources->NumberOfNamedEntries;
		end = start + resources->NumberOfIdEntries;

		while (end > start)
		{
			middle = (start + end) >> 1;
			WORD entryName = (WORD)entries[middle].Name;
			
			if (check < entryName)
			{
				end = (end != middle ? middle : middle - 1);
			}
			else if (check > entryName)
			{
				start = (start != middle ? middle : middle + 1);
			}
			else
			{
				result = &entries[middle];
				break;
			}
		}
	}
	else
	{
		LPCWSTR searchKey;
		size_t searchKeyLen = _tcslen(key);
#if defined(UNICODE)
		searchKey = key;
#else
		// Resource names are always stored using 16bit characters, need to
		// convert string we search for.
#define MAX_LOCAL_KEY_LENGTH 2048
		// In most cases resource names are short, so optimize for that by
		// using a pre-allocated array.
		wchar_t _searchKeySpace[MAX_LOCAL_KEY_LENGTH + 1];
		LPWSTR _searchKey;

		if (searchKeyLen > MAX_LOCAL_KEY_LENGTH)
		{
			size_t _searchKeySize = (searchKeyLen + 1) * sizeof(wchar_t);
			_searchKey = (LPWSTR)malloc(_searchKeySize);

			if (_searchKey == NULL)
			{
				SetLastError(ERROR_OUTOFMEMORY);
				return NULL;
			}
		}
		else
		{
			_searchKey = &_searchKeySpace[0];
		}

		//mbstowcs(_searchKey, key, searchKeyLen);
		size_t length = 0;
		mbstowcs_s(&length, _searchKey, searchKeyLen + 1, key, searchKeyLen);
		
		_searchKey[searchKeyLen] = 0;
		searchKey = _searchKey;
#endif
		start = 0;
		end = resources->NumberOfNamedEntries;
		
		while (end > start)
		{
			PIMAGE_RESOURCE_DIR_STRING_U resourceString;
			middle = (start + end) >> 1;
			resourceString = (PIMAGE_RESOURCE_DIR_STRING_U)(((char *)root) + (entries[middle].Name & 0x7FFFFFFF));
			
			size_t cmp = _wcsnicmp(searchKey, resourceString->NameString, resourceString->Length);
			
			if (cmp == 0)
			{
				// Handle partial match
				cmp = searchKeyLen - resourceString->Length;
			}
			if (cmp < 0)
			{
				end = (middle != end ? middle : middle - 1);
			}
			else if (cmp > 0)
			{
				start = (middle != start ? middle : middle + 1);
			}
			else
			{
				result = &entries[middle];
				break;
			}
		}
#if !defined(UNICODE)
		if (searchKeyLen > MAX_LOCAL_KEY_LENGTH)
		{
			free(_searchKey);
		}
#undef MAX_LOCAL_KEY_LENGTH
#endif
	}

	return result;
}
