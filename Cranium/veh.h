#pragma once
#include "pch.h"
LPCVOID redir_func_ptr;
LPCVOID orig_func_ptr;

namespace VEH
{
	LONG ExcHandler(EXCEPTION_POINTERS* ExceptionInfo)
	{
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE)
		{
			if (ExceptionInfo->ContextRecord->Rip == (DWORD64)orig_func_ptr)
			{
				ExceptionInfo->ContextRecord->Rip = (DWORD64)redir_func_ptr;
			}
			ExceptionInfo->ContextRecord->EFlags |= 0x100; // http://www.c-jump.com/CIS77/ASM/Instructions/I77_0070_eflags_bits.htm (bit 8)
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
		{
			DWORD oldFlags;
			VirtualProtect((LPVOID)orig_func_ptr, 1, PAGE_GUARD | PAGE_EXECUTE_READ, &oldFlags);
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
				if (AddVectoredExceptionHandler(1, ExcHandler))
				{
					DWORD oldFlags;
					VirtualProtect((LPVOID)orig_func_ptr, 1, PAGE_GUARD | PAGE_EXECUTE_READ, &oldFlags);
				}
			}
		}
	}
}
