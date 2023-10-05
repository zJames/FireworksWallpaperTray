//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////

namespace Hooking
{
//------------------------------------------------------------------------------
//	Hooking Allocation Functions
//------------------------------------------------------------------------------
void* AllocPageInTargetProcess(HANDLE process);
void* AllocPage();
void* AllocatePageNearAddressRemote(HANDLE handle, void* targetAddr);
void* AllocatePageNearAddress(void* targetAddr);

} //end namespace Hooking
