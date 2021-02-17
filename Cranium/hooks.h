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
		/*auto CurlEasyAdd = Util::FindPattern(Patterns::Curl::Oldies::CurlEasySetOpt, Masks::Curl::Oldies::CurlEasySetOpt);

		if (CurlEasyAdd)
		{
			
			CurlVsetoptAddress = Util::FindPattern(Patterns::Curl::Oldies::CurlSetOpt, Masks::Curl::Oldies::CurlSetOpt);
			VALIDATE_ADDRESS(CurlVsetoptAddress, XOR("Failed to find Curl SetOpt Address."));

			CurlEasySetOpt = decltype(CurlEasySetOpt)(CurlEasyAdd);
			CurlSetOpt = decltype(CurlSetOpt)(CurlVsetoptAddress);

			//m_pCurlVsetoptHook = new Hook(m_pCurlVsetoptAddress, reinterpret_cast<uintptr_t>(CurlVsetoptHook));
		}
		else
		{*/
			CurlVsetoptAddress = Util::FindPattern(Patterns::Curl::CurlSetOpt, Masks::Curl::CurlSetOpt);
			VALIDATE_ADDRESS(CurlVsetoptAddress, XOR("Failed to find Curl SetOpt Address."));

			CurlSetOpt = decltype(CurlSetOpt)(CurlVsetoptAddress);
			
		    CurlVsetoptHook = new VEH::Hook(CurlVsetoptAddress, reinterpret_cast<uintptr_t>(CurlVsetoptHook));
		//}

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
		//Should work on everything
		auto ProcessEventAdd = Util::FindPattern(Patterns::bGlobal::ProcessEvent, Masks::bGlobal::ProcessEvent);
		VALIDATE_ADDRESS(ProcessEventAdd, XOR("Failed to find ProcessEvent Address."));

		ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);

		gProcessEventAdd = ProcessEventAdd;

		//Used for Camera Hooking.
		//Tested from 12.41 to latest
		auto GetViewPointAdd = Util::FindPattern(Patterns::bGlobal::GetViewPoint, Masks::bGlobal::GetViewPoint);
		VALIDATE_ADDRESS(GetViewPointAdd, XOR("Failed to find GetViewPoint Address."));

		GetViewPoint = decltype(GetViewPoint)(GetViewPointAdd);

		//Used for getting UObjects names.
		//Tested from 12.41 to latest
		auto GONIAdd = Util::FindPattern(Patterns::bGlobal::GONI, Masks::bGlobal::GONI);
		VALIDATE_ADDRESS(GONIAdd, XOR("Failed to find GetObjectName Address."));

		GetObjectNameInternal = decltype(GetObjectNameInternal)(GONIAdd);

		//Used for getting UObjects full names.
		if (gVersion == "12.41" || gVersion == "12.50" || gVersion == "12.61" || gVersion == "13.00")
		{
			//Tested only on 12.41 and 12.61.
			auto GetObjectFullNameAdd = Util::FindPattern(Patterns::Oldies::bGlobal::GetObjectFullName, Masks::Oldies::bGlobal::GetObjectFullName);
			VALIDATE_ADDRESS(GetObjectFullNameAdd, XOR("Failed to find GetObjectFullName Address."));

			GetObjectFullNameInternal = decltype(GetObjectFullNameInternal)(GetObjectFullNameAdd);
		}
		else
		{
			//14.30^
			auto GetObjectFullNameAdd = Util::FindPattern(Patterns::bGlobal::GetObjectFullName, Masks::bGlobal::GetObjectFullName);
			VALIDATE_ADDRESS(GetObjectFullNameAdd, XOR("Failed to find GetObjectFullName Address."));

			GetObjectFullNameInternal = decltype(GetObjectFullNameInternal)(GetObjectFullNameAdd);
		}

		//Used for getting FFields full names.
		auto GetFullNameAdd = Util::FindPattern(Patterns::bGlobal::GetFullName, Masks::bGlobal::GetFullName);
		VALIDATE_ADDRESS(GetFullNameAdd, XOR("Failed to find GetFullName Address."));

		GetFullName = decltype(GetFullName)(GetFullNameAdd);


		//Used to free the memory for names.
		auto FreeInternalAdd = Util::FindPattern(Patterns::bGlobal::FreeInternal, Masks::bGlobal::FreeInternal);
		VALIDATE_ADDRESS(FreeInternalAdd, XOR("Failed to find Free Address."));

		FreeInternal = decltype(FreeInternal)(FreeInternalAdd);


		//Used to construct objects, mostly used for console stuff.
		//Tested from 12.41 to latest
		auto SCOIAdd = Util::FindPattern(Patterns::bGlobal::SCOI, Masks::bGlobal::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, XOR("Failed to find SCOI Address."));

		StaticConstructObject = decltype(StaticConstructObject)(SCOIAdd);


		//Used for mostly everything.
		//Tested from 12.41 to latest
		auto GEngineAdd = Util::FindPattern(Patterns::bGlobal::GEngine, Masks::bGlobal::GEngine);
		VALIDATE_ADDRESS(GEngineAdd, XOR("Failed to find GEngine Address."));

		GEngine = *reinterpret_cast<UEngine**>(GEngineAdd + 7 + *reinterpret_cast<int32_t*>(GEngineAdd + 3));


		//Used to find objects, dump them, mostly works as an alternative for the ObjectFinder.
		auto GObjectsAdd = Util::FindPattern(Patterns::bGlobal::GObjects, Masks::bGlobal::GObjects);
		VALIDATE_ADDRESS(GObjectsAdd, XOR("Failed to find GObjects Address."));

		GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));


		auto AbilityPatchAdd = Util::FindPattern(Patterns::bGlobal::AbilityPatch, Masks::bGlobal::AbilityPatch);
		VALIDATE_ADDRESS(AbilityPatchAdd, XOR("Failed to find AbilityPatch Address."));

		//Patches fortnite ability ownership checks, work on everysingle fortnite version.
		//Author: @nyamimi
		DWORD dwProtection;
		VirtualProtect(reinterpret_cast<void*>(AbilityPatchAdd), 16, PAGE_EXECUTE_READWRITE, &dwProtection);

		reinterpret_cast<uint8_t*>(AbilityPatchAdd)[2] = 0x85;
		reinterpret_cast<uint8_t*>(AbilityPatchAdd)[11] = 0x8D;

		DWORD dwTemp;
		VirtualProtect(reinterpret_cast<void*>(AbilityPatchAdd), 16, dwProtection, &dwTemp);

		//Process Event Hooking.
		MH_CreateHook(reinterpret_cast<void*>(ProcessEventAdd), ProcessEventDetour, reinterpret_cast<void**>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<void*>(ProcessEventAdd));

		//GetViewPoint Hooking.
		MH_CreateHook(reinterpret_cast<void*>(GetViewPointAdd), GetViewPointDetour, reinterpret_cast<void**>(&GetViewPoint));
		MH_EnableHook(reinterpret_cast<void*>(GetViewPointAdd));

		return true;
	}
}
