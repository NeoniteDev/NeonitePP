#pragma once
#include "finder.h"

//TODO: add safety checks in UFuncs.
namespace UFunctions
{
	//same as summon command in-game but from code.
	//UFunctions::Summon(L"Trap_Wall_BouncePad_C");
	inline void Summon(const wchar_t* ClassToSummon)
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(L"GameInstance").Find(L"LocalPlayers");

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(L"PlayerController");

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(L"CheatManager");

		const auto Summon = FindObject<UFunction*>(L"Function /Script/Engine.CheatManager:Summon");

		const FString ClassName = ClassToSummon;

		UCheatManager_Summon_Params params;
		params.ClassName = ClassName;

		ProcessEvent(CheatManagerFinder.GetObj(), Summon, &params);
	}

	//destroys all hlods
	inline void DestroyAllHLODs()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(L"GameInstance").Find(L"LocalPlayers");

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(L"PlayerController");

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(L"CheatManager");

		const auto DestroyAll = FindObject<UFunction*>(L"Function /Script/Engine.CheatManager:DestroyAll");

		const auto HLODSMActor = FindObject<UClass*>(L"Class /Script/FortniteGame.FortHLODSMActor");

		UCheatManager_DestroyAll_Params params;
		params.Class = HLODSMActor;

		ProcessEvent(CheatManagerFinder.GetObj(), DestroyAll, &params);
	}

	//travel to a umap
	inline void Travel(const wchar_t* umap)
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(L"GameInstance").Find(L"LocalPlayers");

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(L"PlayerController");

		const auto SwitchLevel = FindObject<UFunction*>(L"Function /Script/Engine.PlayerController:SwitchLevel");

		const FString URL = umap;

		APlayerController_SwitchLevel_Params params;
		params.URL = URL;

		ProcessEvent(PlayerControllerFinder.GetObj(), SwitchLevel, &params);
		
	}
}

namespace Console
{
	//constructs and assigns FortCheatManager to the main console.
	inline bool CheatManager()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(L"GameInstance").Find(L"LocalPlayers");

		if (!LocalPlayer.GetObj()) return false;

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(L"PlayerController");

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(L"CheatManager");

		UCheatManager*& pcCheatManager = reinterpret_cast<UCheatManager*&>(CheatManagerFinder.GetObj());

		const auto cCheatManager = FindObject<UClass*>(L"Class /Script/FortniteGame.FortCheatManager");

		if (!pcCheatManager && cCheatManager)
		{
			UCheatManager* CheatManager = reinterpret_cast<UCheatManager*>(StaticConstructObject(
				cCheatManager,
				PlayerControllerFinder.GetObj(),
				nullptr,
				RF_NoFlags,
				None,
				nullptr,
				false,
				nullptr,
				false
			));

			pcCheatManager = CheatManager;
			return true;
		}
		return false;
	}

	//unlocks ue4 console with cheat manager ;)
	inline bool Unlock()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder ConsoleClassFinder = EngineFinder.Find(L"ConsoleClass");
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(L"GameViewport");
		ObjectFinder ViewportConsoleFinder = GameViewPortClientFinder.Find(L"ViewportConsole");

		UConsole*& ViewportConsole = reinterpret_cast<UConsole*&>(ViewportConsoleFinder.GetObj());

		UConsole* Console = reinterpret_cast<UConsole*>(StaticConstructObject(
			static_cast<UClass*>(ConsoleClassFinder.GetObj()),
			reinterpret_cast<UObject*>(GameViewPortClientFinder.GetObj()),
			nullptr,
			RF_NoFlags,
			None,
			nullptr,
			false,
			nullptr,
			false
		));

		ViewportConsole = Console;

		CheatManager();
		return true;
	}
}
