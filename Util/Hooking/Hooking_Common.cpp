//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Hooking_Common.h"

#include <cstdint>
#include <cstdio>
#include <psapi.h>
#include <tlhelp32.h>
////////////////////////////////////////////////////////////////////////////////

namespace Hooking
{
//------------------------------------------------------------------------------
// Common functions
//------------------------------------------------------------------------------
bool IsProcess64Bit(HANDLE process)
{
	BOOL isWow64 = false;

	IsWow64Process(process, &isWow64);

	if (isWow64)
	{
		//process is 32 bit, running on 64 bit machine
		return false;
	}
	else
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		return sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64;
	}
}

void RebaseVirtualDrivePath(const char* path, char* outBuff, size_t outBuffSize)
{
	memset(outBuff, 0, outBuffSize);

	char driveLetter[3] = { path[0], path[1], 0};
	char deviceDrive[512];

	QueryDosDevice(driveLetter, deviceDrive, ARRAY_LENGTH(deviceDrive));

	const char* virtualDrivePrefix = "\\??\\";
	char* prefix = strstr(deviceDrive, virtualDrivePrefix);
	
	if (prefix)
	{
		size_t replacementLen = strlen(deviceDrive) - strlen(virtualDrivePrefix);
		size_t rebasedPathLen = replacementLen + strlen(path) - 2;
		
		if (rebasedPathLen >= outBuffSize)
		{
			DebugBreak();
			return;
		}
		
		memcpy(outBuff, deviceDrive + strlen(virtualDrivePrefix), replacementLen);
		memcpy(outBuff + replacementLen, &path[2], strlen(path) - 2);
	}
	else
	{
		if (strlen(path) >= outBuffSize)
		{
			DebugBreak();
			return;
		}

		memcpy(outBuff, path, strlen(path));
	}
}

RefString RebaseVirtualDrivePath(const char* path)
{
	RefString outBuff;

	char driveLetter[3] = { path[0], path[1], 0 };
	RefString deviceDrive(512);
	
	QueryDosDevice(driveLetter, deviceDrive.ptr(), DWORD(deviceDrive.dataSize()));

	const char* virtualDrivePrefix = "\\??\\";
	char* prefix = strstr(deviceDrive.ptr(), virtualDrivePrefix);

	if (prefix)
	{
		size_t replacementLen = strlen(deviceDrive) - strlen(virtualDrivePrefix);
		size_t rebasedPathLen = replacementLen + strlen(path) - 2;

		outBuff.copy(deviceDrive.const_ptr() + strlen(virtualDrivePrefix), replacementLen);
		outBuff.copy(&path[2], strlen(path) - 2);
	}
	else
	{
		outBuff.copy(path, strlen(path));
	}

	return outBuff;
}

Maybe<HMODULE> FindModuleInProcess(HANDLE process, RefString name)
{
	RefString lowerCaseName = name.copy();
	_strlwr_s(lowerCaseName.ptr(), lowerCaseName.length());

	HMODULE remoteProcessModules[1024];
	DWORD numBytesWrittenInModuleArray = 0;
	BOOL success = EnumProcessModules(process, remoteProcessModules, sizeof(HMODULE) * 1024, &numBytesWrittenInModuleArray);

	if (!success)
	{
		fprintf(
			stderr,
			"Error enumerating modules on target process. Error Code %lu \n",
			GetLastError());
		DebugBreak();
	}

	DWORD numRemoteModules = numBytesWrittenInModuleArray / sizeof(HMODULE);
	RefString remoteProcessName(256);
	
	GetModuleFileNameEx(process, NULL, remoteProcessName.ptr(), 256); //a null module handle gets the process name
	_strlwr_s(remoteProcessName.ptr(), remoteProcessName.dataSize());

	MODULEINFO remoteProcessModuleInfo;
	HMODULE remoteProcessModule = 0; //An HMODULE is just the DLL's base address 

	for (DWORD i = 0; i < numRemoteModules; ++i)
	{
		RefString moduleName(256);
		RefString absoluteModuleName(256);
		RefString rebasedPath(256);
		
		GetModuleFileNameEx(process, remoteProcessModules[i], moduleName.ptr(), 256);
		
		_strlwr_s(moduleName.ptr(), moduleName.dataSize());
		const char* lastSlash = strrchr(moduleName.const_ptr(), '\\');
		
		if (!lastSlash)
		{
			lastSlash = strrchr(moduleName, '/');
		}

		const char* dllName = lastSlash + 1;

		if (strcmp(dllName, lowerCaseName) == 0)
		{
			remoteProcessModule = remoteProcessModules[i];

			success =
				GetModuleInformation(
					process,
					remoteProcessModules[i],
					&remoteProcessModuleInfo,
					sizeof(MODULEINFO));

			if (!success)
			{
				DebugBreak();
				return nothing;
			}

			return remoteProcessModule;
		}
		
		//the following string operations are to account for cases where GetModuleFileNameEx
		//returns a relative path rather than an absolute one, the path we get to the module
		//is using a virtual drive letter (ie: one created by subst) rather than a real drive
		char* error =
			_fullpath(absoluteModuleName.ptr(),
				moduleName,
				absoluteModuleName.dataSize());
		
		if (!error)
		{
			DebugBreak();
			return nothing;
		}
	}

	return nothing;
}

void PrintModulesForProcess(HANDLE process)
{
	HMODULE remoteProcessModules[1024];
	DWORD numBytesWrittenInModuleArray = 0;
	BOOL success =
		EnumProcessModules(
			process,
			remoteProcessModules,
			sizeof(HMODULE) * ARRAY_LENGTH(remoteProcessModules),
			&numBytesWrittenInModuleArray);

	if (!success)
	{
		fprintf(
			stderr,
			"Error enumerating modules on target process. Error Code %lu\n",
			GetLastError());
		DebugBreak();
	}

	DWORD numRemoteModules = numBytesWrittenInModuleArray / sizeof(HMODULE);
	HMODULE remoteProcessModule = 0; //An HMODULE is just the DLL's base address 

	for (DWORD i = 0; i < numRemoteModules; ++i)
	{
		RefString moduleName(256);
		RefString absoluteModuleName(256);
		
		GetModuleFileNameEx(
			process,
			remoteProcessModules[i],
			moduleName.ptr(),
			DWORD(moduleName.dataSize()));

		//the following string operations are to account for cases where GetModuleFileNameEx
		//returns a relative path rather than an absolute one, the path we get to the module
		//is using a virtual drive letter (ie: one created by subst) rather than a real drive
		char* error =
			_fullpath(
				absoluteModuleName.ptr(),
				moduleName.ptr(),
				absoluteModuleName.dataSize());
		
		if (!error)
		{
			DebugBreak();
			return;
		}
		
		printf(absoluteModuleName + "\n");
	}
}

Maybe<HMODULE> GetBaseModuleForProcess(HANDLE process)
{
	HMODULE remoteProcessModules[1024];
	DWORD numBytesWrittenInModuleArray = 0;
	BOOL success =
		EnumProcessModules(
			process,
			remoteProcessModules,
			sizeof(HMODULE) * 1024,
			&numBytesWrittenInModuleArray);

	if (!success)
	{
		fprintf(
			stderr,
			"Error enumerating modules on target process. Error Code %lu\n",
			GetLastError());
		DebugBreak();
	}

	DWORD numRemoteModules = numBytesWrittenInModuleArray / sizeof(HMODULE);
	RefString remoteProcessName(256);
	
	GetModuleFileNameEx(
		process,
		NULL,
		remoteProcessName.ptr(),
		DWORD(remoteProcessName.dataSize())); //a null module handle gets the process name
	_strlwr_s(remoteProcessName.ptr(), remoteProcessName.dataSize());

	MODULEINFO remoteProcessModuleInfo;
	HMODULE remoteProcessModule = 0; //An HMODULE is just the DLL's base address 

	for (DWORD i = 0; i < numRemoteModules; ++i)
	{
		RefString moduleName(256);

		GetModuleFileNameEx(
			process,
			remoteProcessModules[i],
			moduleName.ptr(),
			DWORD(moduleName.dataSize()));

		//the following string operations are to account for cases where GetModuleFileNameEx
		//returns a relative path rather than an absolute one, the path we get to the module
		//is using a virtual drive letter (ie: one created by subst) rather than a real drive
		RefString absoluteModuleName(256);

		char* error = _fullpath(absoluteModuleName.ptr(), moduleName, absoluteModuleName.dataSize());
		
		if (!error)
		{
			DebugBreak();
			return nothing;
		}

		RefString rebasedPath = RebaseVirtualDrivePath(absoluteModuleName);
		
		_strlwr_s(rebasedPath.ptr(), rebasedPath.dataSize());

		if (strcmp(remoteProcessName, rebasedPath) == 0)
		{
			remoteProcessModule = remoteProcessModules[i];

			success =
				GetModuleInformation(
					process,
					remoteProcessModules[i],
					&remoteProcessModuleInfo,
					sizeof(MODULEINFO));
			
			if (!success)
			{
				fprintf(
					stderr,
					"Error getting module information for remote process module\n");
				DebugBreak();
			}
			break;
		}
	}

	return remoteProcessModule;
}

Maybe<DWORD> FindPidByName(RefString name)
{
	HANDLE handle =
		CreateToolhelp32Snapshot( //takes a snapshot of specified processes
			TH32CS_SNAPPROCESS, //get all processes
			0); //ignored for SNAPPROCESS

	PROCESSENTRY32 singleProcess = { 0 };

	singleProcess.dwSize = sizeof(PROCESSENTRY32);

	do
	{
		if (name == singleProcess.szExeFile)
		{
			DWORD pid = singleProcess.th32ProcessID;
			CloseHandle(handle);
			return pid;
		}

	} while (Process32Next(handle, &singleProcess));

	CloseHandle(handle);

	return nothing;
}

HMODULE FindModuleBaseAddress(HANDLE process, RefString targetModule)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(process, hMods, sizeof(hMods), &cbNeeded))
	{
		for (uint32_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR moduleName[MAX_PATH];

			// Get the full path to the module's file.
			if (GetModuleFileNameEx(process, hMods[i], moduleName, ARRAY_LENGTH(moduleName)))
			{
				// Print the module name and handle value.
				if (strstr(moduleName, targetModule) != nullptr)
				{
					return hMods[i];
				}
			}
		}
	}

	return NULL;
}

void* FindAddressOfRemoteDLLFunction(HANDLE process, RefString dllName, RefString funcName)
{
	//first, load the dll into this process so we can use GetProcAddress to determine the offset
	//of the target function from the DLL base address
	HMODULE localDLL = LoadLibraryEx(dllName, NULL, 0);
	
	if (!localDLL)
	{
		DebugBreak();
		return nullptr;
	}
	
	void* localHookFunc = GetProcAddress(localDLL, funcName);

	if (!localHookFunc)
	{
		DebugBreak();
		return nullptr;
	}

	uint64_t offsetOfHookFunc = uint64_t(localHookFunc) - uint64_t(localDLL);
	FreeLibrary(localDLL); //free the library, we don't need it anymore.

	//Technically, we could just use the result of GetProcAddress, since in 99% of cases, the base address of the dll
	//in the two processes will be shared thanks to ASLR, but just in case the remote process has relocated the dll, 
	//I'm getting it here separately.

	HMODULE remoteModuleBase = FindModuleBaseAddress(process, dllName);

	return (void*)(uint64_t(remoteModuleBase) + offsetOfHookFunc);
}

void SetOtherThreadsSuspended(bool suspend)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te = { 0 };
		te.dwSize = sizeof(THREADENTRY32);

		if (!Thread32First(hSnapshot, &te))
		{
			return;
		}

		do
		{
			if (te.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(DWORD))
				&& te.th32OwnerProcessID == GetCurrentProcessId()
				&& te.th32ThreadID != GetCurrentThreadId())
			{

				HANDLE thread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
					
				if (thread != NULL)
				{
					if (suspend)
					{
						SuspendThread(thread);
					}
					else
					{
						ResumeThread(thread);
					}
					CloseHandle(thread);
				}
			}
		} while (Thread32Next(hSnapshot, &te));
	}
}

} //end namespace Hooking
