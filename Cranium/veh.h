#pragma once
#include "pch.h"
uintptr_t CurlSetAdd;
LPCVOID redir_func_ptr;
LPCVOID orig_func_ptr;
DWORD oldFlags;
void* ExecHandle;

namespace VEH
{
	long ExcHandler(EXCEPTION_POINTERS* ExceptionInfo)
	{
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE)
		{
			if (ExceptionInfo->ContextRecord->Rip == reinterpret_cast<DWORD64>(orig_func_ptr))
			{
				ExceptionInfo->ContextRecord->Rip = reinterpret_cast<DWORD64>(redir_func_ptr);
			}
			ExceptionInfo->ContextRecord->EFlags |= 0x100; // http://www.c-jump.com/CIS77/ASM/Instructions/I77_0070_eflags_bits.htm (bit 8)
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
		{
			DWORD oldFlags;
			VirtualProtect(const_cast<LPVOID>(orig_func_ptr), 1, PAGE_GUARD | PAGE_EXECUTE_READ, &oldFlags);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	inline void EnableHook(const void* origFuncPtr, const void* redirFuncPtr)
	{
		MEMORY_BASIC_INFORMATION redirMemInfo;
		MEMORY_BASIC_INFORMATION origMemInfo;

		redir_func_ptr = redirFuncPtr;
		orig_func_ptr = origFuncPtr;
		if (VirtualQuery(orig_func_ptr, &origMemInfo, sizeof(origMemInfo)) && VirtualQuery(redir_func_ptr, &redirMemInfo, sizeof(redirMemInfo)))
		{
			if (origMemInfo.BaseAddress != redirMemInfo.BaseAddress)
			{
				ExecHandle = AddVectoredExceptionHandler(1, ExcHandler);
				
				if (ExecHandle)
				{
					VirtualProtect(const_cast<LPVOID>(orig_func_ptr), 1, PAGE_GUARD | PAGE_EXECUTE_READ, &oldFlags);
				}
			}
		}
	}

	inline void DisableHook()
	{
		DWORD dwOldProtect;

		if (VirtualProtect(const_cast<LPVOID>(orig_func_ptr), 1, oldFlags, &dwOldProtect))
		{
			RemoveVectoredExceptionHandler(ExecHandle);
		}
	}
}
