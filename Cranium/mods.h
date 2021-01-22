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
		printf("\n[Neoroyale] %ls was summoned!.\n", ClassToSummon);
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
		printf("\n[Neoroyale] Server is ready to start match now!.\n");
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
		printf("\n[Neoroyale] Match started!.\n");
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

		const auto cCheatManager = FindObject<UClass*>(XOR(L"Class /Script/Engine.CheatManager"));

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
	auto Possess()
	{
		if (gVersion == XOR("15.21"))
		{
			UFunctions::Summon(L"Athena_PlayerController_C");
			const auto PlayerController = FindObject<UObject*>(L"Athena_PlayerController_C /Game/Athena/Apollo/Maps/Apollo_Terrain.Apollo_Terrain:PersistentLevel.Athena_PlayerController_C_");

			const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Controller:Possess"));

			AController_Possess_Params params;

			params.InPawn = reinterpret_cast<UObject*>(this);

			ProcessEvent(PlayerController, fn, &params);
			printf("\n[Neoroyale] PlayerPawn was possessed using %ls!.\n", GetObjectFullName(PlayerController).c_str());
			return;
		}

		ObjectFinder EngineFinder = ObjectFinder::GetEngine(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Controller:Possess"));

		AController_Possess_Params params;
		params.InPawn = reinterpret_cast<UObject*>(this);

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
		printf("\n[Neoroyale] PlayerPawn was possessed!.\n");
	}

	auto StartSkydiving(float height)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawnAthena:TeleportToSkyDive"));

		AFortPlayerPawnAthena_TeleportToSkyDive_Params params;
		params.HeightAboveGround = height;

		ProcessEvent(this, fn, &params);
		printf("\nSkydiving!, Redeploying at %fm.\n", height);
	}

	auto IsJumpProvidingForce()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:IsJumpProvidingForce"));

		ACharacter_IsJumpProvidingForce_Params params;

		ProcessEvent(this, fn, &params);

		return params.ReturnValue;
	}

	auto Jump()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:Jump"));

		Empty_Params params;

		ProcessEvent(this, fn, &params);
	}

	auto SetSkeletalMesh()
	{
		ObjectFinder PawnFinder = ObjectFinder::GetEngine(uintptr_t(this));
		ObjectFinder MeshFinder = PawnFinder.Find(XOR(L"Mesh"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.SkinnedMeshComponent:SetSkeletalMesh"));

		const auto Mesh = FindObject<UObject*>(
			XOR(L"SkeletalMesh /Game/Characters/Player/Male/Medium/Base/SK_M_MALE_Base.SK_M_MALE_Base")
		);

		if (Mesh)
		{
			USkinnedMeshComponent_SetSkeletalMesh_Params params;
			params.NewMesh = Mesh;
			params.bReinitPose = false;

			ProcessEvent(MeshFinder.GetObj(), fn, &params);
		}
	}

	auto ShowSkin()
	{
		ObjectFinder PawnFinder = ObjectFinder::GetEngine(uintptr_t(this));
		ObjectFinder PlayerStateFinder = PawnFinder.Find(XOR(L"PlayerState"));

		const auto KismetLib = FindObject<UObject*>(XOR(L"FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortKismetLibrary:UpdatePlayerCustomCharacterPartsVisualization"));

		UFortKismetLibrary_UpdatePlayerCustomCharacterPartsVisualization_Params params;
		params.PlayerState = PlayerStateFinder.GetObj();

		ProcessEvent(KismetLib, fn, &params);
		printf("\nCharacter parts should be visiable now!.\n");
	}

	auto EquipWeapon(const wchar_t* weaponname, int guid)
	{
		FGuid GUID;
		GUID.A = guid;
		GUID.B = guid;
		GUID.C = guid;
		GUID.D = guid;

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:EquipWeaponDefinition"));

		const auto WeaponData = FindObject<UObject*>(weaponname);

		if (WeaponData)
		{
			//weapon found lets equip it
			AFortPawn_EquipWeaponDefinition_Params params;
			params.WeaponData = WeaponData;
			params.ItemEntryGuid = GUID;

			ProcessEvent(this, fn, &params);
		}
	}

	auto Fly(bool bIsFlying)
	{
		ObjectFinder PawnFinder = ObjectFinder::GetEngine(uintptr_t(this));

		ObjectFinder CharMovementFinder = PawnFinder.Find(XOR(L"CharacterMovement"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CharacterMovementComponent:SetMovementMode"));

		UCharacterMovementComponent_SetMovementMode_Params params;

		if (!bIsFlying) params.NewMovementMode = EMovementMode::MOVE_Flying;
		else params.NewMovementMode = EMovementMode::MOVE_Walking;
		params.NewCustomMode = 0;

		ProcessEvent(CharMovementFinder.GetObj(), fn, &params);
	}
};

namespace Neoroyale
{
	inline bool bIsInit;
	inline bool bIsStarted;
	inline bool bHasJumped;
	inline Pawn* PlayerPawn;

	inline void start()
	{
		if (gVersion == XOR("15.21"))
		{
			UFunctions::Travel(APOLLO_TERRAIN_BASE);
		}
		else
		{
			UFunctions::Travel(APOLLO_TERRAIN);
			bIsStarted = !bIsStarted;
		}
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

		PlayerPawn = reinterpret_cast<Pawn*>(FindActor(L"PlayerPawn_Athena_C"));

		if (PlayerPawn)
		{
			PlayerPawn->Possess();

			if (gVersion != XOR("15.21"))
			{
				UFunctions::ServerReadyToStartMatch();

				UFunctions::StartMatch();
			}

			//PlayerPawn->SetSkeletalMesh();

			PlayerPawn->ShowSkin();

			PlayerPawn->StartSkydiving(300.0f);

			PlayerPawn->EquipWeapon(
				XOR(L"FortWeaponRangedItemDefinition /Game/Athena/Items/Weapons/WID_AshtonPack_Indigo.WID_AshtonPack_Indigo"),
				0);

			CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&thread), nullptr, NULL, nullptr);
		}

		bIsInit = !bIsInit;
	}
}
