#include "veh.h"

#define XIP Rip

namespace VEH
{
	uintptr_t Hook::m_pTarget = 0;
	uintptr_t Hook::m_pDetour = 0;

	Hook::Hook(uintptr_t pTarget, uintptr_t pDetour)
	{
		Hook::m_pTarget = pTarget;
		Hook::m_pDetour = pDetour;

		if (!IsSamePage(reinterpret_cast<const PBYTE>(pTarget), reinterpret_cast<const PBYTE>(pDetour)))
		{
			m_hHandle = AddVectoredExceptionHandler(true, static_cast<PVECTORED_EXCEPTION_HANDLER>(Handler));

			if (m_hHandle &&
				VirtualProtect(reinterpret_cast<LPVOID>(pTarget), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &m_dwProtect))
			{
				bSuccess = true;
			}
		}
	}
	Hook::~Hook() {
		DWORD dwOldProtect;

		if (m_hHandle &&
			VirtualProtect(reinterpret_cast<LPVOID>(m_pTarget), 1, m_dwProtect, &dwOldProtect) &&
			RemoveVectoredExceptionHandler(m_hHandle))
		{
			bSuccess = true;
		} else {
			bSuccess = false;
		}
	}

	BOOL Hook::IsSamePage(const PBYTE pFirstAddr, const PBYTE pSecondAddr)
	{
		MEMORY_BASIC_INFORMATION mbiFirst{}, mbiSecond{};

		// Query both the first and second address.
		if (!VirtualQuery(pFirstAddr, &mbiFirst, sizeof(mbiFirst)))
		{
			return true;
		}
		if (!VirtualQuery(pSecondAddr, &mbiSecond, sizeof(mbiSecond)))
		{
			return true;
		}

		if (mbiFirst.BaseAddress == mbiSecond.BaseAddress)
		{
			return true;
		}

		return false;
	}

	LONG WINAPI Hook::Handler(EXCEPTION_POINTERS* pException)
	{
		DWORD dwOldProtect;

		switch (pException->ExceptionRecord->ExceptionCode)
		{
		case STATUS_GUARD_PAGE_VIOLATION:
			if (pException->ContextRecord->XIP == m_pTarget) {
				pException->ContextRecord->XIP = m_pDetour;
			}

			pException->ContextRecord->EFlags |= 0x100;

			return EXCEPTION_CONTINUE_EXECUTION;

		case STATUS_SINGLE_STEP:
			VirtualProtect(reinterpret_cast<LPVOID>(m_pTarget), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);

			return EXCEPTION_CONTINUE_EXECUTION;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}
