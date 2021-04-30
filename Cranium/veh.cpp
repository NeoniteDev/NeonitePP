#include "framework.h"
#include "veh.h"

namespace VEH
{
	bool IsSamePage(void* A, void* B)
	{
		MEMORY_BASIC_INFORMATION InfoA;
		if (!VirtualQuery(A, &InfoA, sizeof(InfoA)))
		{
			return true;
		}

		MEMORY_BASIC_INFORMATION InfoB;
		if (!VirtualQuery(B, &InfoB, sizeof(InfoB)))
		{
			return true;
		}

		return InfoA.BaseAddress == InfoB.BaseAddress;
	}

	struct HOOK_INFO
	{
		void* Original;
		void* Detour;

		HOOK_INFO(void* Original, void* Detour) :
			Original(Original),
			Detour(Detour)
		{
		}
	};

	std::vector<HOOK_INFO> Hooks;
	std::vector<DWORD> HookProtections;
	HANDLE ExceptionHandler;

	LONG WINAPI Handler(EXCEPTION_POINTERS* Exception)
	{
		thread_local void* OriginalHook = nullptr;

		if (Exception->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
		{
			auto Itr = std::find_if(Hooks.begin(), Hooks.end(),
				[Rip = Exception->ContextRecord->Rip](const HOOK_INFO& Hook)
			{
				return Hook.Original == (void*)Rip;
			});
			if (Itr != Hooks.end())
			{
				OriginalHook = Itr->Original;
				Exception->ContextRecord->Rip = (uintptr_t)Itr->Detour;
			}

			Exception->ContextRecord->EFlags |= 0x100; // SINGLE_STEP_FLAG

			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else if (Exception->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
		{
			for (auto& Hook : Hooks)
			{
				DWORD dwOldProtect;
				VirtualProtect(Hook.Original, 1, PAGE_EXECUTE_READ | PAGE_GUARD,
					&dwOldProtect);
			}

			return EXCEPTION_CONTINUE_EXECUTION;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	bool Init()
	{
		if (ExceptionHandler == nullptr)
		{
			ExceptionHandler = AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)Handler);
		}
		return ExceptionHandler != nullptr;
	}

	bool AddHook(void* Target, void* Detour)
	{
		if (ExceptionHandler == nullptr)
		{
			return false;
		}
		if (IsSamePage(Target, Detour))
		{
			return false;
		}
		if (!VirtualProtect(Target, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &HookProtections.emplace_back()))
		{
			HookProtections.pop_back();
			return false;
		}

		Hooks.emplace_back(Target, Detour);
		return true;
	}

	bool RemoveHook(void* Original)
	{
		auto Itr = std::find_if(Hooks.begin(), Hooks.end(), [Original](const HOOK_INFO& Hook)
			{
				return Hook.Original == Original;
			});

		if (Itr == Hooks.end())
		{
			return false;
		}

		const auto ProtItr = HookProtections.begin() + std::distance(Hooks.begin(), Itr);
		Hooks.erase(Itr);

		DWORD dwOldProtect;
		bool Ret = VirtualProtect(Original, 1, *ProtItr, &dwOldProtect);
		HookProtections.erase(ProtItr);

		return false;
	}
}
