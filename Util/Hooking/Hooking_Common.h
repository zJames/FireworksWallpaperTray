//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "..\Containers\Maybe.h"
#include "..\Text\RefString.h"

#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////

namespace Hooking
{
//------------------------------------------------------------------------------
//	Common Functions
//------------------------------------------------------------------------------
bool IsProcess64Bit(HANDLE process);

//returns the first module called "name" -> only searches for dll name, not whole path
//ie: somepath/subdir/mydll.dll can be searched for with "mydll.dll"
Maybe<HMODULE> FindModuleInProcess(HANDLE process, RefString name);

void PrintModulesForProcess(HANDLE process);
Maybe<HMODULE> GetBaseModuleForProcess(HANDLE process);
Maybe<DWORD> FindPidByName(RefString name);
HMODULE FindModuleBaseAddress(HANDLE process, RefString targetModule);
void* FindAddressOfRemoteDLLFunction(HANDLE process, RefString dllName, RefString funcName);
void SetOtherThreadsSuspended(bool suspend);

} //end namespace Hooking
