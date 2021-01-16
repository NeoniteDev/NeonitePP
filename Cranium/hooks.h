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

	inline bool curl()
	{
		//cURL Hooking.
		//Works on all versions
		const auto CurlEasyAdd = Util::FindPattern(Patterns::CurlEasySetOpt, Masks::CurlEasySetOpt);
		VALIDATE_ADDRESS(CurlEasyAdd, XOR("Failed to find Curl Easy Address."));

		const auto CurlSetAdd = Util::FindPattern(Patterns::CurlSetOpt, Masks::CurlSetOpt);
		VALIDATE_ADDRESS(CurlSetAdd, XOR("Failed to find Curl SetOpt Address."));

		CurlEasySetOpt = decltype(CurlEasySetOpt)(CurlEasyAdd);
		CurlSetOpt = decltype(CurlSetOpt)(CurlSetAdd);

		VEH::EnableHook(CurlEasySetOpt, CurlEasySetOptDetour);
		return true;
	}

#endif

	inline bool Misc(std::string version)
	{
		if (MH_Initialize() != MH_OK)
		{
			MessageBoxA(nullptr, XOR("Failed to initialize min-hook, terminating the thread."), XOR("Cranium"), MB_OK);
			FreeLibraryAndExitThread(GetModuleHandle(nullptr), 0);
		}

		//Used for ProcessEvent Hooking.
		//Tested from 12.41 to latest
		const auto ProcessEventAdd = Util::FindPattern(Patterns::ProcessEvent, Masks::ProcessEvent);
		VALIDATE_ADDRESS(ProcessEventAdd, XOR("Failed to find ProcessEvent Address."));

		ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);

		//Used for Camera Hooking.
		//Tested from 12.41 to latest
		const auto GetViewPointAdd = Util::FindPattern(Patterns::GetViewPoint, Masks::GetViewPoint);
		VALIDATE_ADDRESS(GetViewPointAdd, XOR("Failed to find GetViewPoint Address."));

		GetViewPoint = decltype(GetViewPoint)(GetViewPointAdd);

		//Used for getting UObjects names.
		//Tested from 12.41 to latest
		const auto GONIAdd = Util::FindPattern(Patterns::GONI, Masks::GONI);
		VALIDATE_ADDRESS(GONIAdd, XOR("Failed to find GetObjectName Address."));

		GetObjectNameInternal = decltype(GetObjectNameInternal)(GONIAdd);

		//Used for getting UObjects full names.
		if (gVersion == "12.41")
		{
			const auto GetObjectFullNameAdd = Util::FindPattern(Patterns::GetObjectFullName12_41, Masks::GetObjectFullName12_41);
			VALIDATE_ADDRESS(GetObjectFullNameAdd, XOR("Failed to find GetObjectFullName Address."));

			GetObjectFullNameInternal = decltype(GetObjectFullNameInternal)(GetObjectFullNameAdd);
		}
		else
		{
			//14.30^
			const auto GetObjectFullNameAdd = Util::FindPattern(Patterns::GetObjectFullName, Masks::GetObjectFullName);
			VALIDATE_ADDRESS(GetObjectFullNameAdd, XOR("Failed to find GetObjectFullName Address."));

			GetObjectFullNameInternal = decltype(GetObjectFullNameInternal)(GetObjectFullNameAdd);
		}

		//Used for getting FFields full names.
		const auto GetFullNameAdd = Util::FindPattern(Patterns::GetFullName, Masks::GetFullName);
		VALIDATE_ADDRESS(GetFullNameAdd, XOR("Failed to find GetFullName Address."));

		GetFullName = decltype(GetFullName)(GetFullNameAdd);


		//Used to construct objects, mostly used for console stuff.
		//Tested from 12.41 to latest
		const auto SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, XOR("Failed to find SCOI Address."));

		StaticConstructObject = decltype(StaticConstructObject)(SCOIAdd);


		//Used for mostly everything.
		if (gVersion == "12.41")
		{
			const auto GEngineAdd = Util::FindPattern(Patterns::GEngine12_41, Masks::GEngine12_41);

			VALIDATE_ADDRESS(GEngineAdd, XOR("Failed to find GEngine Address."));

			GEngine = *reinterpret_cast<UEngine**>(GEngineAdd + 7 + *reinterpret_cast<int32_t*>(GEngineAdd + 3));
		}
		else
		{
			//14.30^
			const auto GEngineAdd = Util::FindPattern(Patterns::GEngine, Masks::GEngine);
			VALIDATE_ADDRESS(GEngineAdd, XOR("Failed to find GEngine Address."));

			GEngine = *reinterpret_cast<UEngine**>(GEngineAdd + 22 + *reinterpret_cast<int32_t*>(GEngineAdd + 18));
		}
		
		//Used to find objects, dump them, mostly works as an alternative for the ObjectFinder.
		if (gVersion == "12.41")
		{
			const auto GObjectsAdd = Util::FindPattern(Patterns::GObjects12_41, Masks::GObjects12_41);
			VALIDATE_ADDRESS(GObjectsAdd, XOR("Failed to find GObjects Address."));
			GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));
		}
		else
		{
			//14.30^
			const auto GObjectsAdd = Util::FindPattern(Patterns::GObjects, Masks::GObjects);
			VALIDATE_ADDRESS(GObjectsAdd, XOR("Failed to find GObjects Address."));
			GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));
		}


		//Process Event Hooking.
		MH_CreateHook(reinterpret_cast<void*>(ProcessEventAdd), ProcessEventDetour, reinterpret_cast<void**>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<void*>(ProcessEventAdd));

		//GetViewPoint Hooking.
		MH_CreateHook(reinterpret_cast<void*>(GetViewPointAdd), GetViewPointDetour, reinterpret_cast<void**>(&GetViewPoint));
		MH_EnableHook(reinterpret_cast<void*>(GetViewPointAdd));
		
		return true;
	}
}
