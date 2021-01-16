#pragma once
#include "finder.h"

//TODO: add safety checks in UFuncs.
namespace UFunctions
{
	//same as summon command in-game but from code.
	inline void Summon(const wchar_t* ClassToSummon)
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:Summon"));

		const FString ClassName = ClassToSummon;

		UCheatManager_Summon_Params params;
		params.ClassName = ClassName;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	inline void DestroyAllHLODs()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:DestroyAll"));

		const auto HLODSMActor = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.FortHLODSMActor"));

		UCheatManager_DestroyAll_Params params;
		params.Class = HLODSMActor;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	//travel to a url
	inline void Travel(const wchar_t* url)
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.PlayerController:SwitchLevel"));

		const FString URL = url;

		APlayerController_SwitchLevel_Params params;
		params.URL = URL;

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
	}

	//Simulates the server telling the game that it's ready to start match
	inline void ServerReadyToStartMatch()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerController:ServerReadyToStartMatch"));

		Empty_Params params;

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
	}

	//Read the name lol
	inline void StartMatch()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameModeFinder = WorldFinder.Find(L"AuthorityGameMode");

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameMode:StartMatch"));

		Empty_Params params;

		ProcessEvent(GameModeFinder.GetObj(), fn, &params);
	}


	inline void Play(const wchar_t* AnimationPlayerFullName)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/MovieScene.MovieSceneSequencePlayer:Play"));

		const auto Sequence = FindObject<void*>(AnimationPlayerFullName);

		ProcessEvent(Sequence, fn, nullptr);
	}
}

namespace Console
{
	//constructs and assigns FortCheatManager to the main console.
	inline bool CheatManager()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		if (!LocalPlayer.GetObj()) return false;

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		UObject*& pcCheatManager = reinterpret_cast<UObject*&>(CheatManagerFinder.GetObj());

		const auto cCheatManager = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.FortCheatManager"));

		if (!pcCheatManager && cCheatManager)
		{
			const auto CheatManager = StaticConstructObject(
				cCheatManager,
				PlayerControllerFinder.GetObj(),
				nullptr,
				RF_NoFlags,
				None,
				nullptr,
				false,
				nullptr,
				false
			);

			pcCheatManager = CheatManager;
			return true;
		}
		return false;
	}

	//unlocks ue4 console with cheat manager
	inline bool Unlock()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder ConsoleClassFinder = EngineFinder.Find(XOR(L"ConsoleClass"));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder ViewportConsoleFinder = GameViewPortClientFinder.Find(XOR(L"ViewportConsole"));

		UObject*& ViewportConsole = reinterpret_cast<UObject*&>(ViewportConsoleFinder.GetObj());

		const auto Console = StaticConstructObject(
			static_cast<UClass*>(ConsoleClassFinder.GetObj()),
			reinterpret_cast<UObject*>(GameViewPortClientFinder.GetObj()),
			nullptr,
			RF_NoFlags,
			None,
			nullptr,
			false,
			nullptr,
			false
		);

		ViewportConsole = Console;

		CheatManager();
		return true;
	}
}

//TODO: move this from here
struct Pawn
{
	void Possess()
	{
		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Controller:Possess"));

		AController_Possess_Params params;
		params.InPawn = reinterpret_cast<UObject*>(this);

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
	}

	void StartSkydiving(bool bFromBus)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:BeginSkydiving"));

		AFortPlayerPawn_BeginSkydiving_Params params;
		params.bFromBus = bFromBus;

		ProcessEvent(this, fn, &params);
	}

	bool IsJumpProvidingForce()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:IsJumpProvidingForce"));

		ACharacter_IsJumpProvidingForce_Params params;

		ProcessEvent(this, fn, &params);

		return params.ReturnValue;
	}

	void Jump()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:Jump"));

		Empty_Params params;

		ProcessEvent(this, fn, &params);
	}

	auto Test()
	{
	}
};

namespace Neoroyale
{
	inline bool bIsInit;
	inline bool bIsStarted;
	inline bool bHasBuilt;
	inline bool bHasJumped;
	inline Pawn* PlayerPawn;

	inline void start()
	{
		UFunctions::Travel(APOLLO_TERRAIN);
		bIsStarted = !bIsStarted;
	}

	inline void thread()
	{
		while (true)
		{
			if (PlayerPawn && GetAsyncKeyState(VK_SPACE))
			{
				if (!bHasJumped)
				{
					bHasJumped = !bHasJumped;
					if (!PlayerPawn->IsJumpProvidingForce())
					{
						PlayerPawn->Jump();
					}
				}
			}
			else bHasJumped = false;

			if (PlayerPawn && GetAsyncKeyState(VK_LBUTTON))
			{
				if (!bHasBuilt)
				{
					bHasBuilt = !bHasBuilt;
					UFunctions::Summon(L"PBWA_M1_StairW_C");
				}
			}
			else bHasBuilt = false;

			if (GetAsyncKeyState(VK_F3))
			{
				UFunctions::Travel(FRONTEND);
				bIsStarted = false;
				bIsInit = false;
				PlayerPawn = nullptr;
				break;
			}

			Sleep(1000 / 60);
		}
	}


	inline void init()
	{
		UFunctions::Summon(L"PlayerPawn_Athena_C");
		printf("\n[Neoroyale] PlayerPawn was summoned!.\n");

		PlayerPawn = reinterpret_cast<Pawn*>(FindActor(L"PlayerPawn_Athena_C"));

		if (PlayerPawn)
		{
			printf("\n[Neoroyale] PlayerPawn was found!.\n");

			PlayerPawn->Possess();
			printf("\n[Neoroyale] PlayerPawn was possessed!.\n");

			UFunctions::ServerReadyToStartMatch();
			printf("\n[Neoroyale] Server is ready to start match now!.\n");

			UFunctions::StartMatch();
			printf("\n[Neoroyale] Match STARTED!.\n");

			PlayerPawn->StartSkydiving(true);

			CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&thread), nullptr, NULL, nullptr);
		}

		bIsInit = !bIsInit;
	}
}
