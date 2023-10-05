//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Hooking_Alloc.h"

#include "Hooking_Common.h"
////////////////////////////////////////////////////////////////////////////////

namespace Hooking
{
//------------------------------------------------------------------------------
// Alloc
//------------------------------------------------------------------------------
void* AllocPageInTargetProcess(HANDLE process)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int pageSize = sysInfo.dwPageSize;

	return VirtualAllocEx(process, NULL, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void* AllocPage()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int pageSize = sysInfo.dwPageSize;

	return VirtualAlloc(NULL, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void* AllocatePageNearAddressRemote(HANDLE handle, void* targetAddr)
{
	if (!IsProcess64Bit(handle))
	{
		return nullptr;
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	const uint64_t pageSize = sysInfo.dwPageSize;

	uint64_t startAddr = (uint64_t(targetAddr) & ~(pageSize - 1)); //round down to nearest page boundary
	uint64_t minAddr = min(startAddr - 0x7FFFFF00, uint64_t(sysInfo.lpMinimumApplicationAddress));
	uint64_t maxAddr = max(startAddr + 0x7FFFFF00, uint64_t(sysInfo.lpMaximumApplicationAddress));

	uint64_t startPage = (startAddr - (startAddr % pageSize));

	uint64_t pageOffset = 1;
	bool needsExit = false;

	do
	{
		uint64_t byteOffset = pageOffset * pageSize;
		uint64_t highAddr = startPage + byteOffset;
		uint64_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

		needsExit = highAddr > maxAddr && lowAddr < minAddr;

		if (highAddr < maxAddr)
		{
			void* outAddr =
				VirtualAllocEx(
					handle,
					(void*)highAddr,
					size_t(pageSize),
					MEM_COMMIT | MEM_RESERVE,
					PAGE_EXECUTE_READWRITE);

			if (outAddr)
			{
				return outAddr;
			}
		}

		if (lowAddr > minAddr)
		{
			void* outAddr =
				VirtualAllocEx(
					handle,
					(void*)lowAddr,
					size_t(pageSize),
					MEM_COMMIT | MEM_RESERVE,
					PAGE_EXECUTE_READWRITE);

			if (outAddr)
			{
				return outAddr;
			}
		}

		++pageOffset;
	} while (!needsExit);

	return nullptr;
}

void* AllocatePageNearAddress(void* targetAddr)
{
	return AllocatePageNearAddressRemote(GetCurrentProcess(), targetAddr);
}

} //end namespace placeholder
