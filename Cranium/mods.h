#pragma once
#include "finder.h"
#include "pawn.h"

inline UObject* gPlaylist;

//TODO: add safety checks in UFuncs.
namespace UFunctions
{

	auto SetTimeOfDay(float Time)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");

		const auto KismetLib = FindObject<UObject*>(XOR(L"FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortKismetLibrary:SetTimeOfDay"));

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

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

		UCheatManager_BugItGo_Params params;
		params.X = -156128.36;
		params.Y = -159492.78;
		params.Z = -2996.30;
		params.Pitch = 0;
		params.Yaw = 0;
		params.Roll = 0;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	inline void TeleportToMain()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

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

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:BugItGo"));

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

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:DestroyAll"));

		const auto HLODSMActor = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.FortHLODSMActor"));

		UCheatManager_DestroyAll_Params params;
		params.Class = HLODSMActor;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
	}

	//travel to a url
	inline void Travel(const wchar_t* url)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
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
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
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
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameModeFinder = WorldFinder.Find(L"AuthorityGameMode");

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameMode:StartMatch"));

		Empty_Params params;

		ProcessEvent(GameModeFinder.GetObj(), fn, &params);
		printf("\n[Neoroyale] Match started!.\n");
	}

	inline void SetPlaylist()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameStateFinder = WorldFinder.Find(XOR(L"GameState"));

		const auto CurrentPlaylistInfoOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortGameStateAthena"), XOR(L"CurrentPlaylistInfo"));

		const auto CurrentPlaylistInfo = reinterpret_cast<FPlaylistPropertyArray*>(reinterpret_cast<uintptr_t>(GameStateFinder.GetObj()) + CurrentPlaylistInfoOffset);

		CurrentPlaylistInfo->BasePlaylist = gPlaylist;
		CurrentPlaylistInfo->OverridePlaylist = gPlaylist;

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortGameStateAthena:OnRep_CurrentPlaylistInfo"));

		Empty_Params params;

		ProcessEvent(GameStateFinder.GetObj(), fn, &params);
	}

	inline void SetGamePhase()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameStateFinder = WorldFinder.Find(XOR(L"GameState"));

		const auto GamePhaseOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortGameStateAthena"), XOR(L"GamePhase"));

		EAthenaGamePhase* GamePhase = reinterpret_cast<EAthenaGamePhase*>(reinterpret_cast<uintptr_t>(GameStateFinder.GetObj()) + GamePhaseOffset);

		*GamePhase = EAthenaGamePhase::None;

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortGameStateAthena:OnRep_GamePhase"));

		AFortGameStateAthena_OnRep_GamePhase_Params params;
		params.OldGamePhase = EAthenaGamePhase::Setup;

		ProcessEvent(GameStateFinder.GetObj(), fn, &params);
	}

	/*
	inline void AddItemToInventory(AFortInventory* ParentInventory, UObject* ItemToAdd, int ItemCount, FGuid withGUID)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortInventory:HandleInventoryLocalUpdate"));

		Empty_Params params;

		ProcessEvent(Inventory, fn, &params);

        //This will be used to edit it's entries to set it's GUID
        UFortWorldItem* WorldItemInstance;
        
		//This is the instance to be later used in for the Updated Item List. New WorldItems get added to this...
		TArray<UFortItem*> InventoryItemInstances;

		ParentInventory->HandleInventoryLocalUpdate();

        struct FFortItemEntry TempItemEntry;

		TArray<struct FFortItemEntry> Replicated_Entries;

        TempItemEntry.Count = 1;
		TempItemEntry.ItemDefiniton = ItemToAdd;
		TempItemEntry.Durability = 3232.0000;
		TempItemEntry.LoadedAmmo = 32;
		TempItemEntry.ItemGUID = withGUID;


        //Add the New Item Entry to the Replicated Entries custom variable which is to be later used in setting the ReplicatedEntries in the Inventory Structure...
		Replicated_Entries.Add(TempItemEntry);

        //Create a new FortItem which is to be casted to FortWorldItem later on so the Entry can be set for the GUID,Count,Level,Durability and etc...
		WorldItemInstance = static_cast<UFortWorldItem*>(ItemToAdd->CreateTemporaryItemInstanceBP(ItemCount, 3));
        
		//Set the World Item's Count...
        WorldItemInstance -> ItemEntry.Count = 1;

        //Set the GUID of the WorldItem, I don't know if you could generate a random GUID by calling a function... we might have to do this manually
		WorldItemInstance -> ItemEntry.ItemGUID = withGUID;       

        //Add our new FortWorldItem to tne InventoryItemInstances variable which will then be used as the ItemInstances for the updated ItemList of the Inventory...
        InventoryItemInstances.Add(WorldItemInstance);

       //Adds the FortWorldItem that we had created and set GUID and count to the the ItemInstances Array in the Inventory structure in the Parent Inventory.
		ParentInventory -> Inventory.ItemInstances.Add(WorldItemInstance); 

		//Create a new struct to be later set as the Variable InventoryItemInstances
		struct FFortItemList UpdatedItemList;
		//which is then is later used to be the new ItemList for the inventory
        
		//Finish updating the replicated entries by setting it to our newly updated one.
		UpdatedItemList.ReplicatedEntries = ReplicatedEntries;

		//Set the ItemInstances list to the newly created one.
		UpdatedItemList.ItemInstances = InventoryItemInstances;
         
        //Finish setting the Inventory struct (which is a FFortItemList) to our update ItemList;
		ParentInventory -> Inventory = UpdatedItemList;

        ParentInventory ->HandleInventoryLocalUpdate();
		ParentInventory ->ClientForceWorldInventoryUpdate();
		AFortPlayerState* PawnPlayerState -> static_cast<AFortPlayerState*>(Pawn->PlayerState);
		PawnPlayerState -> OnRep_AccumulatedItems();
		PawnPlayerState -> OnRep_QuickbarEquippedItems();

	}*/

	inline void LoadAndStreamInLevel(const wchar_t* EventSequenceMap)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder NetworkManagerFinder = WorldFinder.Find(XOR(L"NetworkManager"));
		ObjectFinder PersistentLevelFinder = WorldFinder.Find(XOR(L"PersistentLevel"));

		//Loading the level instance in memory
		const auto LoadLevelInstance = FindObject<UFunction*>(XOR(L"Function /Script/Engine.LevelStreamingDynamic:LoadLevelInstance"));
		const auto LevelStreamingDynamic = FindObject<UObject*>(XOR(L"LevelStreamingDynamic /Script/Engine.Default__LevelStreamingDynamic"));

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
		printf("\n[DEBUG] LEVEL INSTANCE WAS CREATED\n");

		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));
		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));

		const auto KismetSysLib = FindObject<UObject*>(XOR(L"KismetSystemLibrary /Script/Engine.Default__KismetSystemLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.KismetSystemLibrary:ExecuteConsoleCommand"));

		std::wstring command = L"streamlevelin " + std::wstring(EventSequenceMap);

		UKismetSystemLibrary_ExecuteConsoleCommand_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.Command = command.c_str();
		params.SpecificPlayer = PlayerControllerFinder.GetObj();

		ProcessEvent(KismetSysLib, fn, &params);
	}

	inline void Play(const wchar_t* AnimationPlayerFullName)
	{
		const auto Play = FindObject<UFunction*>(XOR(L"Function /Script/MovieScene.MovieSceneSequencePlayer:Play"));

		const auto Sequence = FindObject<void*>(AnimationPlayerFullName);

		ProcessEvent(Sequence, Play, nullptr);
	}

	inline void ConsoleLog(std::wstring message)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");
		ObjectFinder GameModeFinder = WorldFinder.Find(L"AuthorityGameMode");

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.GameMode:Say"));

		const FString Msg = message.c_str();
		AGameMode_Say_Params params;
		params.Msg = Msg;

		ProcessEvent(GameModeFinder.GetObj(), fn, &params);
	}

	inline void DestoryActor(UObject* actor)
	{
		if (!Util::IsBadReadPtr(actor))
		{
			const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Actor:K2_DestroyActor"));

			ProcessEvent(actor, fn, nullptr);
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
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
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
