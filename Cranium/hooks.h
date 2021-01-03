#pragma once
#include "pch.h"
#include "structs.h"
#include "enums.h"
#include "util.h"
#include "detours.h"
#include "ue4.h"
#include "veh.h"

namespace Hooks
{
#ifdef SSL_BYPASS

	bool curl()
	{
		//cURL Hooking.
		const auto CurlEasyAdd = Util::FindPattern(Patterns::CurlEasySetOpt, Masks::CurlEasySetOpt);
		VALIDATE_ADDRESS(CurlEasyAdd, "Failed to find Curl Easy Address.");

		const auto CurlSetAdd = Util::FindPattern(Patterns::CurlSetOpt, Masks::CurlSetOpt);
		VALIDATE_ADDRESS(CurlSetAdd, "Failed to find Curl SetOpt Address.");
		
		CurlEasySetOpt = decltype(CurlEasySetOpt)(CurlEasyAdd);
		CurlSetOpt = decltype(CurlSetOpt)(CurlSetAdd);
		
		VEH::EnableHook(CurlEasySetOpt, CurlEasySetOptDetour);
		return true;
	}

#endif

	bool init()
	{
		if (MH_Initialize() != MH_OK)
		{
			MessageBoxA(0, "Failed to initialize min-hook, terminating the thread.", "Cranium", MB_OK);
			FreeLibraryAndExitThread(GetModuleHandle(nullptr), 0);
		}

		//Used for ProcessEvent Hooking.
		const auto ProcessEventAdd = Util::FindPattern(Patterns::ProcessEvent, Masks::ProcessEvent);
		VALIDATE_ADDRESS(ProcessEventAdd, "Failed to find ProcessEvent Address.");

		ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);

		//Used for Camera Hooking.
		const auto GetViewPointAdd = Util::FindPattern(Patterns::GetViewPoint, Masks::GetViewPoint);
		VALIDATE_ADDRESS(GetViewPointAdd, "Failed to find GetViewPoint Address.");
		
		GetViewPoint = decltype(GetViewPoint)(GetViewPointAdd);

		//Used for getting UObjects names.
		const auto GONIAdd = Util::FindPattern(Patterns::GONI, Masks::GONI);
		VALIDATE_ADDRESS(GONIAdd, "Failed to find GetObjectName Address.");

		GetObjectNameInternal = decltype(GetObjectNameInternal)(GONIAdd);

		//Used for getting UObjects full names.
		const auto GetObjectFullNameAdd = Util::FindPattern(Patterns::GetObjectFullName, Masks::GetObjectFullName);
		VALIDATE_ADDRESS(GetObjectFullNameAdd, "Failed to find GetObjectName Address.");

		GetObjectFullNameInternal = decltype(GetObjectFullNameInternal)(GetObjectFullNameAdd);

		//Used for getting FFields full names.
		const auto GetFullNameAdd = Util::FindPattern(Patterns::GetFullName, Masks::GetFullName);
		VALIDATE_ADDRESS(GetFullNameAdd, "Failed to find GetFullName Address.");

		GetFullName = decltype(GetFullName)(GetFullNameAdd);

		//Used for mostly everything.
		const auto GEngineAdd = Util::FindPattern(Patterns::GEngine, Masks::GEngine);
		VALIDATE_ADDRESS(GEngineAdd, "Failed to find GEngine Address.");

		GEngine = *reinterpret_cast<UEngine**>(GEngineAdd + 22 + *reinterpret_cast<int32_t*>(GEngineAdd + 18));

		//Used to find objects, dump them, mostly works as an alternative for the ObjectFinder.
		const auto GObjectsAdd = Util::FindPattern(Patterns::GObjects, Masks::GObjects);
		VALIDATE_ADDRESS(GObjectsAdd, "Failed to find GObjects Address.");

		GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));

		//Used to construct objects, mostly used for console stuff.
		const auto SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, "Failed to find SCOI Address.");

		StaticConstructObject = decltype(StaticConstructObject)(SCOIAdd);

		//Process Event Hooking.
		MH_CreateHook(reinterpret_cast<void*>(ProcessEventAdd), ProcessEventDetour, reinterpret_cast<void**>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<void*>(ProcessEventAdd));

		//GetViewPoint Hooking.
		MH_CreateHook(reinterpret_cast<void*>(GetViewPointAdd), GetViewPointDetour, reinterpret_cast<void**>(&GetViewPoint));
		MH_EnableHook(reinterpret_cast<void*>(GetViewPointAdd));
		
		return true;
	}
}
