#pragma once
#include "pch.h"
#include <winscard.h>

#define VALIDATE_ADDRESS(address, error) \
    if (!address) { \
        MessageBoxA(0, error, "Cranium", MB_OK); \
        FreeLibraryAndExitThread(GetModuleHandle(NULL), 0); \
        return 0; \
    }

#define RELATIVE_ADDRESS(address, size) ((PBYTE)((UINT_PTR)(address) + *(PINT)((UINT_PTR)(address) + ((size) - sizeof(INT))) + (size)))

class Util
{
private:
	static __forceinline bool MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
	{
		for (PBYTE value = reinterpret_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
		{
			if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value) return false;
		}
		return true;
	}

public:
	static __forceinline uintptr_t FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
	{
		dwSize -= static_cast<DWORD>(strlen(lpMask));
		for (unsigned long index = 0; index < dwSize; ++index)
		{
			PBYTE pAddress = reinterpret_cast<PBYTE>(pBase) + index;
			if (MaskCompare(pAddress, lpPattern, lpMask)) return reinterpret_cast<uintptr_t>(pAddress);
		}
		return NULL;
	}

	static __forceinline uintptr_t FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
	{
		MODULEINFO info = {nullptr};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &info, sizeof(info));
		return FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
	}
};
