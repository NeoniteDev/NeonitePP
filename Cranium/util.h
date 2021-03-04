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

#define READ_POINTER(base, offset) (*(PVOID *)(((PBYTE)base + offset)))

#define READ_DWORD(base, offset) (*(PDWORD)(((PBYTE)base + offset)))

#define GET_POINTER(base, offset) *reinterpret_cast<PVOID*>((static_cast<PBYTE>(base) + offset));

class Util
{
private:
	static __forceinline bool MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
	{
		for (PBYTE value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
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
			PBYTE pAddress = static_cast<PBYTE>(pBase) + index;
			if (MaskCompare(pAddress, lpPattern, lpMask)) return reinterpret_cast<uintptr_t>(pAddress);
		}
		return NULL;
	}

	static __forceinline uintptr_t FindPattern(LPCSTR lpPattern, LPCSTR lpMask, BOOL SleepBetween = false)
	{
		MODULEINFO info = {nullptr};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &info, sizeof(info));

		uintptr_t pAddr = 0;

		do
		{
			pAddr = FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);

			Sleep(50);
		}
		while (!pAddr);

		return pAddr;
	}

	static __forceinline std::wstring sSplit(std::wstring s, std::wstring delimiter)
	{
		size_t pos;
		std::wstring token;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			return token;
		}
		return token;
	}

	static __forceinline void CopyToClipboard(const std::string& s)
	{
		OpenClipboard(nullptr);
		EmptyClipboard();
		const auto hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
		if (!hg)
		{
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}

	static __forceinline bool IsBadReadPtr(void* p)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery(p, &mbi, sizeof(mbi)))
		{
			DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool b = !(mbi.Protect & mask);
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

			return b;
		}
		return true;
	}
};
