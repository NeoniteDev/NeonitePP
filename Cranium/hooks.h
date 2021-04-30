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

	inline bool Init()
	{
		//<REDACTED>

		
		return true;
	}

#endif

	inline bool Misc(float version)
	{
		if (MH_Initialize() != MH_OK)
		{
			MessageBoxA(nullptr, XOR("Failed to initialize min-hook, terminating the thread."), XOR("Cranium"), MB_OK);
			FreeLibraryAndExitThread(GetModuleHandle(nullptr), 0);
		}

		//GObject Array
		auto GObjectsAdd = Util::FindPattern(Patterns::bGlobal::GObjects, Masks::bGlobal::GObjects);
		VALIDATE_ADDRESS(GObjectsAdd, XOR("Failed to find GObjects Address."));

		GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));

		auto FNameToStringAdd = Util::FindPattern(Patterns::New::FNameToString,
		                                          Masks::New::FNameToString);
		VALIDATE_ADDRESS(FNameToStringAdd, XOR("Failed to find FNameToString Address."));

		/*const auto offset = *reinterpret_cast<int32_t*>(FNameToStringAdd + 1);
		FNameToStringAdd = FNameToStringAdd + 5 + offset;*/

		FNameToString = decltype(FNameToString)(FNameToStringAdd);

		//A work around instead of using a pattern.
		GEngine = UE4::FindObject<UEngine*>(XOR(L"FortEngine /Engine/Transient.FortEngine_"));


		uintptr_t ProcessEventAdd;
		if (version >= 16.00f)
		{
			const auto vtable = *reinterpret_cast<void***>(GEngine);
			ProcessEventAdd = (uintptr_t)vtable[0x44];
		}
		else
		{
			ProcessEventAdd = Util::FindPattern(Patterns::bGlobal::ProcessEvent, Masks::bGlobal::ProcessEvent);
			VALIDATE_ADDRESS(ProcessEventAdd, XOR("Failed to find ProcessEvent Address."));
		}


		ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);
		MH_CreateHook(reinterpret_cast<void*>(ProcessEventAdd), ProcessEventDetour,
		              reinterpret_cast<void**>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<void*>(ProcessEventAdd));


		//Used to construct objects, mostly used for console stuff.
		//Tested from 12.41 to latest
		auto SCOIAdd = Util::FindPattern(Patterns::bGlobal::SCOI, Masks::bGlobal::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, XOR("Failed to find SCOI Address."));

		StaticConstructObject = decltype(StaticConstructObject)(SCOIAdd);


		//Used to load objects.
		auto SLOIAdd = Util::FindPattern(Patterns::bGlobal::SLOI, Masks::bGlobal::SLOI);
		VALIDATE_ADDRESS(SLOIAdd, XOR("Failed to find SLOI Address."));

		StaticLoadObject = decltype(StaticLoadObject)(SLOIAdd);


		//Used to spawn actors
		auto SpawnActorAdd = Util::FindPattern(Patterns::bGlobal::SpawnActorInternal,
		                                       Masks::bGlobal::SpawnActorInternal);
		VALIDATE_ADDRESS(SpawnActorAdd, XOR("Failed to find SpawnActor Address."));

		SpawnActor = decltype(SpawnActor)(SpawnActorAdd);


		//Commented out as of s16
		/*
		auto AbilityPatchAdd = Util::FindPattern(Patterns::bGlobal::AbilityPatch, Masks::bGlobal::AbilityPatch);
		VALIDATE_ADDRESS(AbilityPatchAdd, XOR("Failed to find AbilityPatch Address."));
		MessageBoxW(nullptr, L"abilitypatch", L"test", MB_OK);
		
		//Patches fortnite ability ownership checks, work on everysingle fortnite version.
		//Author: @nyamimi
		reinterpret_cast<uint8_t*>(AbilityPatchAdd)[2] = 0x85;
		reinterpret_cast<uint8_t*>(AbilityPatchAdd)[11] = 0x8D;

		//Used for Camera Hooking.
		//Tested from 12.41 to latest
		auto GetViewPointAdd = Util::FindPattern(Patterns::bGlobal::GetViewPoint, Masks::bGlobal::GetViewPoint);
		VALIDATE_ADDRESS(GetViewPointAdd, XOR("Failed to find GetViewPoint Address."));
		

		GetViewPoint = decltype(GetViewPoint)(GetViewPointAdd);

	    //GetViewPoint Hooking.
		MH_CreateHook(reinterpret_cast<void*>(GetViewPointAdd), GetViewPointDetour, reinterpret_cast<void**>(&GetViewPoint));
		MH_EnableHook(reinterpret_cast<void*>(GetViewPointAdd));
		*/

		std::wstring map;
		printf("\n\n\n\nPlease enter the map name you want to open: \n\n\n\n");
		getline(std::wcin, map);
		//Start(map.c_str());
		UFunctions::Travel(map.c_str());

		return true;
	}
}
