//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////

namespace Hooking
{
//------------------------------------------------------------------------------
//	Hooking Write Functions
//------------------------------------------------------------------------------
uint32_t WriteMovToRCX(uint8_t* dst, uint64_t val);
uint32_t WriteSaveArgumentRegisters(uint8_t* dst);
uint32_t WriteRestoreArgumentRegisters(uint8_t* dst);
uint32_t WriteAddRSP32(uint8_t* dst);
uint32_t WriteSubRSP32(uint8_t* dst);
uint32_t WriteAbsoluteCall64(uint8_t* dst, void* funcToCall);
uint32_t WriteAbsoluteJump64(void* absJumpMemory, void* addrToJumpTo);
uint32_t WriteAbsoluteJump64(HANDLE process, void* absJumpMemory, void* addrToJumpTo);
uint32_t WriteRelativeJump(void* func2hook, void* jumpTarget);
uint32_t WriteRelativeJump(void* func2hook, void* jumpTarget, uint8_t numTrailingNOPs);
uint32_t WriteRelativeJump(HANDLE process, void* func2hook, void* jumpTarget);

} //end namespace Hooking
