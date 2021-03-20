#pragma once

#include "mods.h"
#include "server.h"

inline std::vector<std::wstring> gWeapons;
inline std::vector<std::wstring> gBlueprints;
inline std::vector<std::wstring> gMeshes;
inline std::vector<Player> Bots;

namespace NeoRoyale
{
	inline bool bIsInit;
	inline bool bIsStarted;
	inline bool bIsPlayerInit;

	inline bool bHasJumped;
	inline bool bHasShowedPickaxe;

	inline bool bWantsToJump;
	inline bool bWantsToSkydive;
	inline bool bWantsToOpenGlider;
	inline bool bWantsToShowPickaxe;

	inline Player NeoPlayer;

	inline void Start(const wchar_t* MapToPlayOn)
	{
		UFunctions::Travel(MapToPlayOn);
		bIsStarted = !bIsStarted;
	}

	inline void Stop()
	{
		UFunctions::Travel(FRONTEND);
		bIsStarted = false;
		bIsInit = false;
		NeoPlayer.Controller = nullptr;
		NeoPlayer.Pawn = nullptr;
		NeoPlayer.Mesh = nullptr;
		NeoPlayer.AnimInstance = nullptr;
		Bots.clear();
		gPlaylist = nullptr;
		gNeoniteLogoTexture = nullptr;
	}

	inline void LoadMoreClasses()
	{
		const auto BPGClass = UE4::FindObject<UClass*>(XOR(L"Class /Script/Engine.BlueprintGeneratedClass"));

		//Mech
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Athena/DrivableVehicles/Mech/TestMechVehicle.TestMechVehicle_C"));

		//Husks
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Characters/Enemies/Husk/Blueprints/HuskPawn.HuskPawn_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Characters/Enemies/DudeBro/Blueprints/DUDEBRO_Pawn.DUDEBRO_Pawn_C"));


		//CameraFilters
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_FilmNoir.PP_FilmNoir_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Crazy.PP_Crazy_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Dark.PP_Dark_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_HappyPlace.PP_HappyPlace_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Oak.PP_Oak_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Pixelizer.PP_Pixelizer_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Red.PP_Red_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Retro.PP_Retro_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Sepia.PP_Sepia_C"));
		UE4::StaticLoadObjectEasy(BPGClass, XOR(L"/Game/Creative/PostProcess/PP_Spooky.PP_Spooky_C"));
	}

	inline void InitCombos()
	{
		for (auto i = 0x0; i < GObjs->NumElements; ++i)
		{
			auto object = GObjs->GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}

			if (!Util::IsBadReadPtr(object))
			{
				auto objectFullName = object->GetFullName();
				auto objectFirstName = object->GetName();

				if ((objectFullName.starts_with(L"AthenaGadget") || objectFirstName.starts_with(L"WID_")) && !objectFirstName.starts_with(L"Default__"))
				{
					gWeapons.push_back(objectFirstName);
				}
				else if (objectFirstName.ends_with(L"_C") && !objectFirstName.starts_with(L"Default__"))
				{
					gBlueprints.push_back(objectFirstName);
				}
				else if (objectFullName.starts_with(L"SkeletalMesh ") && !objectFirstName.starts_with(L"Default__"))
				{
					gMeshes.push_back(objectFirstName);
				}
			}
		}
	}

	inline void Thread()
	{
		//NOTE (kemo): i know this isn't the best practice but it does the job on another thread so it's not a frezzing call
		while (true)
		{
			if (NeoPlayer.Pawn && GetAsyncKeyState(VK_SPACE))
			{
				if (!bHasJumped)
				{
					bHasJumped = !bHasJumped;
					if (!NeoPlayer.IsInAircraft())
					{
						if (NeoPlayer.IsSkydiving() && !NeoPlayer.IsParachuteOpen() && !NeoPlayer.IsParachuteForcedOpen())
						{
							bWantsToOpenGlider = true;
						}


						else if (NeoPlayer.IsSkydiving() && NeoPlayer.IsParachuteOpen() && !NeoPlayer.IsParachuteForcedOpen())
						{
							bWantsToSkydive = true;
						}


						else if (!NeoPlayer.IsJumpProvidingForce())
						{
							bWantsToJump = true;
						}
					}
				}
			}
			else bHasJumped = false;


			if (NeoPlayer.Pawn && GetAsyncKeyState(0x31) /* 1 key */)
			{
				if (!NeoPlayer.IsInAircraft())
				{
					if (!bHasShowedPickaxe)
					{
						bHasShowedPickaxe = !bHasShowedPickaxe;
						NeoPlayer.StopMontageIfEmote();
						bWantsToShowPickaxe = true;
					}
				}
			}
			else bHasShowedPickaxe = false;


			if (NeoPlayer.Pawn && GetAsyncKeyState(VK_F3))
			{
				Stop();
				break;
			}

			Sleep(1000 / 30);
		}
	}

	inline void Init()
	{
		UFunctions::DestroyAll(UE4::FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.FortHLODSMActor")));

		NeoPlayer.Pawn = UE4::SpawnActorEasy(UE4::FindObject<UClass*>(XOR(L"BlueprintGeneratedClass /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C")));

		NeoPlayer.Authorize();

		if (NeoPlayer.Pawn)
		{
			NeoPlayer.Possess();

			NeoPlayer.ShowSkin();

			NeoPlayer.ShowPickaxe();

			NeoPlayer.ToggleInfiniteAmmo();

			NeoPlayer.ApplyOverride();

			NeoPlayer.SetMovementSpeed(1.1);

			auto PlaylistName = gPlaylist->GetName();

			if (!wcsstr(PlaylistName.c_str(), XOR(L"Playlist_Papaya")) &&
				!wcsstr(PlaylistName.c_str(), XOR(L"Playlist_BattleLab")))
			{
				NeoPlayer.TeleportToSpawn();
			}

			if (gVersion > 14.60)
			{
				UFunctions::SetPlaylist();

				UFunctions::SetGamePhase();
			}

			if (gVersion == 14.60f)
			{
				UFunctions::LoadAndStreamInLevel(GALACTUS_EVENT_MAP);
			}
			else if (gVersion == 12.41f)
			{
				UFunctions::LoadAndStreamInLevel(JERKY_EVENT_MAP);
			}
			else if (gVersion == 12.61f)
			{
				UFunctions::LoadAndStreamInLevel(DEVICE_EVENT_MAP);
			}

			InitCombos();

			UFunctions::StartMatch();

			UFunctions::ServerReadyToStartMatch();

			CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&Thread), nullptr, NULL, nullptr);

			UFunctions::ConsoleLog(XOR(L"\n\nWelcome to Neonite++\nMade with ♥ By Kemo (@xkem0x on twitter)."));

			//ConnectServer();

			bIsInit = !bIsInit;
		}
	}
}
