#pragma once
#include "finder.h"
#include "kismet.h"
#include "neonitelogo.h"
#include "pawn.h"

inline UObject* gPlaylist;
inline UObject* gNeoniteLogoTexture;

inline bool ForceSettings()
{
	auto FortGameUserSetttings = FindObject<UObject*>(XOR(L"FortGameUserSettings /Engine/Transient.FortGameUserSettings_"));

	if (FortGameUserSetttings)
	{
		auto SetFullscreenMode = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameUserSettings:SetFullscreenMode"));

		UGameUserSettings_SetFullscreenMode_Params SetFullscreenMode_Params;
		SetFullscreenMode_Params.InFullscreenMode = EWindowMode::WindowedFullscreen;

		ProcessEvent(FortGameUserSetttings, SetFullscreenMode, &SetFullscreenMode_Params);


		auto SaveSettings = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameUserSettings:SaveSettings"));

		ProcessEvent(FortGameUserSetttings, SaveSettings, nullptr);

		return true;
	}

	return false;
}

//TODO: add safety checks in UFuncs.
namespace UFunctions
{
	auto SetTimeOfDay(float Time)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));

		auto KismetLib = FindObject<UObject*>(XOR(L"FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortKismetLibrary:SetTimeOfDay"));

		UFortKismetLibrary_SetTimeOfDay_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.TimeOfDay = Time;

		ProcessEvent(KismetLib, fn, &params);
	}

	inline void TeleportToSpawn()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

		UCheatManager_BugItGo_Params params;
		params.X = -156128.36;
		params.Y = -159492.78;
		params.Z = -2996.30;
		params.Pitch = 0;
		params.Yaw = 0;
		params.Roll = 0;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);

		printf(XOR("\n[NeoRoyale] Teleported to spawn island.\n"));
	}

	inline void TeleportToMain()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

		UCheatManager_BugItGo_Params params;
		params.X = 0;
		params.Y = 0;
		params.Z = 0;
		params.Pitch = 0;
		params.Yaw = 0;
		params.Roll = 0;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	inline void TeleportToCoords(float X, float Y, float Z)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

		UCheatManager_BugItGo_Params params;
		params.X = X;
		params.Y = Y;
		params.Z = Z;
		params.Pitch = 0;
		params.Yaw = 0;
		params.Roll = 0;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	inline void DestroyAllHLODs()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:DestroyAll"));

		auto HLODSMActor = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.FortHLODSMActor"));

		UCheatManager_DestroyAll_Params params;
		params.Class = HLODSMActor;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
		printf(XOR("[NeoRoyale] HLODSM Actor was destroyed."));
	}

	//travel to a url
	inline void Travel(const wchar_t* url)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.PlayerController:SwitchLevel"));

		const FString URL = url;

		APlayerController_SwitchLevel_Params params;
		params.URL = URL;

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
	}

	//Read the name lol
	inline void StartMatch()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameModeFinder = WorldFinder.Find(L"AuthorityGameMode");
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameMode:StartMatch"));
		Empty_Params params;
		ProcessEvent(GameModeFinder.GetObj(), fn, &params);
		printf("\n[Neoroyale] Match started!.\n");
	}

	//Simulates the server telling the game that it's ready to start match
	inline void ServerReadyToStartMatch()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerController:ServerReadyToStartMatch"));

		Empty_Params params;

		ProcessEvent(PlayerControllerFinder.GetObj(), fn, &params);
		printf(XOR("\n[NeoRoyale] Server is now ready to start match!\n"));
	}

	inline void SetPlaylist()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));
		ObjectFinder GameStateFinder = WorldFinder.Find(XOR(L"GameState"));

		auto CurrentPlaylistInfoOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortGameStateAthena"), XOR(L"CurrentPlaylistInfo"));

		auto CurrentPlaylistInfo = reinterpret_cast<FPlaylistPropertyArray*>(reinterpret_cast<uintptr_t>(GameStateFinder.GetObj()) + CurrentPlaylistInfoOffset);

		CurrentPlaylistInfo->BasePlaylist = gPlaylist;
		CurrentPlaylistInfo->OverridePlaylist = gPlaylist;

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortGameStateAthena:OnRep_CurrentPlaylistInfo"));

		Empty_Params params;

		ProcessEvent(GameStateFinder.GetObj(), fn, &params);

		printf(XOR("\n[NeoRoyale] Playlist was set!\n"));
	}

	inline void SetGamePhase()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));
		ObjectFinder GameStateFinder = WorldFinder.Find(XOR(L"GameState"));

		auto GamePhaseOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortGameStateAthena"), XOR(L"GamePhase"));

		EAthenaGamePhase* GamePhase = reinterpret_cast<EAthenaGamePhase*>(reinterpret_cast<uintptr_t>(GameStateFinder.GetObj()) + GamePhaseOffset);

		*GamePhase = EAthenaGamePhase::None;

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortGameStateAthena:OnRep_GamePhase"));

		AFortGameStateAthena_OnRep_GamePhase_Params params;
		params.OldGamePhase = EAthenaGamePhase::Setup;

		ProcessEvent(GameStateFinder.GetObj(), fn, &params);

		printf(XOR("\n[NeoRoyale] Game phase was set!\n"));
	}


	inline void LoadAndStreamInLevel(const wchar_t* EventSequenceMap)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));
		ObjectFinder NetworkManagerFinder = WorldFinder.Find(XOR(L"NetworkManager"));
		ObjectFinder PersistentLevelFinder = WorldFinder.Find(XOR(L"PersistentLevel"));

		//Loading the level instance in memory
		auto LoadLevelInstance = FindObject<UFunction*>(XOR(L"Function /Script/Engine.LevelStreamingDynamic:LoadLevelInstance"));
		auto LevelStreamingDynamic = FindObject<UObject*>(XOR(L"LevelStreamingDynamic /Script/Engine.Default__LevelStreamingDynamic"));

		FRotator WorldRotation;
		WorldRotation.Yaw = 0;
		WorldRotation.Roll = 0;
		WorldRotation.Pitch = 0;

		ULevelStreamingDynamic_LoadLevelInstance_Params LoadLevelInstanceParams;
		LoadLevelInstanceParams.WorldContextObject = WorldFinder.GetObj();
		LoadLevelInstanceParams.LevelName = EventSequenceMap;
		LoadLevelInstanceParams.Location = FVector(0, 0, 0);
		LoadLevelInstanceParams.Rotation = WorldRotation;

		ProcessEvent(LevelStreamingDynamic, LoadLevelInstance, &LoadLevelInstanceParams);

		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));
		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		auto KismetSysLib = FindObject<UObject*>(XOR(L"KismetSystemLibrary /Script/Engine.Default__KismetSystemLibrary"));
		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.KismetSystemLibrary:ExecuteConsoleCommand"));

		std::wstring command = L"streamlevelin " + std::wstring(EventSequenceMap);

		UKismetSystemLibrary_ExecuteConsoleCommand_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.Command = command.c_str();
		params.SpecificPlayer = PlayerControllerFinder.GetObj();

		ProcessEvent(KismetSysLib, fn, &params);
	}

	inline void Play(const wchar_t* AnimationPlayerFullName)
	{
		auto Play = FindObject<UFunction*>(XOR(L"Function /Script/MovieScene.MovieSceneSequencePlayer:Play"));

		auto Sequence = FindObject<void*>(AnimationPlayerFullName);

		ProcessEvent(Sequence, Play, nullptr);
	}

	inline void ConsoleLog(std::wstring message)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));
		ObjectFinder GameModeFinder = WorldFinder.Find(XOR(L"AuthorityGameMode"));

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameMode:Say"));

		const FString Msg = message.c_str();
		AGameMode_Say_Params params;
		params.Msg = Msg;

		ProcessEvent(GameModeFinder.GetObj(), fn, &params);
	}

	inline void DestoryActor(UObject* actor)
	{
		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Actor:K2_DestroyActor"));

		ProcessEvent(actor, fn, nullptr);
	}


	inline void LoadLogoAsTexture()
	{
		//Using cstdio for speed, we can't allocate our tarrays so we write the buffer to file, load, then delete it (under 10ms)
		FILE* file = fopen(XOR("npp.png"), XOR("wb"));
		fwrite(NeoniteLogoBuffer, 1, sizeof NeoniteLogoBuffer, file);
		fclose(file);

		std::wstring filepath = Util::GetRuntimePath() + XOR(L"\\npp.png");

		gNeoniteLogoTexture = KismetFunctions::ImportPngAsTexture2D(filepath.c_str());

		remove(XOR("npp.png"));
	}

	inline void SetBodyCustomTextureFromPng(const wchar_t* PngFileFullPath, bool bIsHead = false)
	{
		const auto SetTextureParameterValue = FindObject<UFunction*>(XOR(L"Function /Script/Engine.MaterialInstanceDynamic:SetTextureParameterValue"));

		UMaterialInstanceDynamic_SetTextureParameterValue_Params params;
		params.ParameterName = KismetFunctions::GetFName(XOR(L"Diffuse"));
		params.Value = KismetFunctions::ImportPngAsTexture2D(PngFileFullPath);

		for (auto i = 0; i < 6; i++)
		{
			auto MaterialInstanceDynamic = FindObject<UObject*>(
				XOR(L"MaterialInstanceDynamic /Game/Athena/Apollo/Maps/Apollo_Terrain.Apollo_Terrain:PersistentLevel.PlayerPawn_Athena_C_"), false, false, i);

			if (!MaterialInstanceDynamic)
			{
				MaterialInstanceDynamic = FindObject<UObject*>(
					XOR(L"MaterialInstanceDynamic /Game/Maps/Frontend.Frontend:PersistentLevel.PlayerPawn_Athena_C_"), false, false, i);
			}

			if (MaterialInstanceDynamic)
			{
				auto toFind = XOR(L"CharacterPartSkelMesh_Body");

				if (bIsHead)
				{
					toFind = XOR(L"CharacterPartSkelMesh_Head");
				}

				if (GetObjectFullName(MaterialInstanceDynamic).find(toFind) != std::wstring::npos)
				{
					ProcessEvent(MaterialInstanceDynamic, SetTextureParameterValue, &params);
					return;
				}
			}
			else break;
		}
	}

	inline void SetImageFromTexture(UObject* Image, UObject* Texture)
	{
		auto SetBrushFromTexture = FindObject<UFunction*>(XOR(L"Function /Script/UMG.Image:SetBrushFromTexture"));

		SetBrushFromTextureParams SetBrushFromTexture_Params;

		SetBrushFromTexture_Params.Texture = Texture;
		SetBrushFromTexture_Params.bMatchSize = false;

		ProcessEvent(Image, SetBrushFromTexture, &SetBrushFromTexture_Params);
	}

	inline void PlayCustomPlayPhaseAlert()
	{
		if (!gNeoniteLogoTexture || Util::IsBadReadPtr(gNeoniteLogoTexture))
		{
			LoadLogoAsTexture();
		}

		auto AGPCW = FindObject<UObject*>(XOR(L"AthenaGamePhaseChangeWidget_C /Engine/Transient.FortEngine_"));

		auto AGPCWFinder = ObjectFinder::EntryPoint(uintptr_t(AGPCW));

		auto IconFinder = AGPCWFinder.Find(XOR(L"Icon"));

		SetImageFromTexture(IconFinder.GetObj(), gNeoniteLogoTexture);

		auto PlayIntroAnim = FindObject<UObject*>(XOR(L"Function /Game/Athena/HUD/Phase/AthenaGamePhaseChangeWidget.AthenaGamePhaseChangeWidget_C:PlayIntroAnimation"));

		PlayIntroAnim_Params PlayIntroAnimParams;

		PlayIntroAnimParams.Step = EAthenaGamePhaseStep::Count;

		ProcessEvent(AGPCW, PlayIntroAnim, &PlayIntroAnimParams);
	}

	inline void SetupCustomInventory()
	{
		if (!gNeoniteLogoTexture || Util::IsBadReadPtr(gNeoniteLogoTexture))
		{
			LoadLogoAsTexture();
		}

		const auto Widget = FindObject<UObject*>(XOR(L"HousepartyMicToast_C /Engine/Transient.FortEngine_"));

		auto WidgetFinder = ObjectFinder::EntryPoint(uintptr_t(Widget));

		auto ImageFinder = WidgetFinder.Find(XOR(L"NotificationImage"));

		SetImageFromTexture(ImageFinder.GetObj(), gNeoniteLogoTexture);

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteUI.AthenaHUDMenu:SetInventoryPanelOverride"));

		const auto Hud = FindObject<UObject*>(XOR(L"AthenaHUDMenu_C /Engine/Transient.FortEngine_"));

		SetInventoryPanelOverride_Params SetInventoryPanelOverrideParams;
		SetInventoryPanelOverrideParams.InInventoryPanelOverride = Widget;

		ProcessEvent(Hud, fn, &SetInventoryPanelOverrideParams);
	}

	inline auto StaticLoadObjectEasy(UClass* inClass, const wchar_t* inName, UObject* inOuter = nullptr)
	{
		return StaticLoadObject(inClass, inOuter, inName, nullptr, 0, nullptr, false, nullptr);
	}

	inline void RegionCheck()
	{
		auto Qos = FindObject<UObject*>(XOR(L"QosRegionManager /Engine/Transient.QosRegionManager_"));

		auto RegionDefinitions = *reinterpret_cast<TArray<FQosRegionInfo>*>(reinterpret_cast<uintptr_t>(Qos) + ObjectFinder::FindOffset(
			XOR(L"Class /Script/Qos.QosRegionManager"), XOR(L"RegionDefinitions")));

		auto RegionId = RegionDefinitions.operator[](0).RegionId.ToString();

		if (!RegionId.starts_with(XOR("NPP")))
		{
			exit(0);
		}
	}
}

namespace Console
{
	//constructs and assigns CheatManager to the main console.
	inline bool CheatManager()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		if (!LocalPlayer.GetObj()) return false;

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		UObject*& pcCheatManager = reinterpret_cast<UObject*&>(CheatManagerFinder.GetObj());

		auto cCheatManager = FindObject<UClass*>(XOR(L"Class /Script/Engine.CheatManager"));

		if (!pcCheatManager && cCheatManager)
		{
			auto CheatManager = StaticConstructObject(
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
			printf(XOR("[NeoRoyale] Player now has cheatmanager!"));
			return true;
		}
		return false;
	}

	//unlocks ue4 console with cheat manager
	inline bool Unlock()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder ConsoleClassFinder = EngineFinder.Find(XOR(L"ConsoleClass"));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder ViewportConsoleFinder = GameViewPortClientFinder.Find(XOR(L"ViewportConsole"));

		UObject*& ViewportConsole = reinterpret_cast<UObject*&>(ViewportConsoleFinder.GetObj());

		auto Console = StaticConstructObject(
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
