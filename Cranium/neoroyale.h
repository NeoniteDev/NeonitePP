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
				auto objectFullName = GetObjectFullName(object);
				auto objectFirstName = GetObjectFirstName(object);

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
		Console::CheatManager();

		UFunctions::DestroyAllHLODs();

		NeoPlayer.Summon(XOR(L"PlayerPawn_Athena_C"));

		NeoPlayer.Pawn = ObjectFinder::FindActor(XOR(L"PlayerPawn_Athena_C"));

		if (NeoPlayer.Pawn)
		{
			NeoPlayer.Possess();

			NeoPlayer.ShowSkin();

			NeoPlayer.ShowPickaxe();

			NeoPlayer.ToggleInfiniteAmmo();

			NeoPlayer.ApplyOverride();

			//LOL
			NeoPlayer.ExecuteConsoleCommand(XOR(L"god"));
			NeoPlayer.SetMovementSpeed(1.1);

			auto PlaylistName = GetObjectFirstName(gPlaylist);

			if (!wcsstr(PlaylistName.c_str(), XOR(L"Playlist_Papaya")) &&
				!wcsstr(PlaylistName.c_str(), XOR(L"Playlist_BattleLab")))
			{
				UFunctions::TeleportToSpawn();
			}
			
			if (gVersion != "12.41" || gVersion != "14.60" || gVersion != "12.61")
			{
				UFunctions::SetPlaylist();

				UFunctions::SetGamePhase();
			}

			if (gVersion == XOR("14.60"))
			{
				UFunctions::LoadAndStreamInLevel(GALACTUS_EVENT_MAP);
			}
			else if (gVersion == XOR("12.41"))
			{
				UFunctions::LoadAndStreamInLevel(JERKY_EVENT_MAP);
			}
			else if (gVersion == XOR("12.61"))
			{
				UFunctions::LoadAndStreamInLevel(DEVICE_EVENT_MAP);
			}

			UFunctions::StartMatch();
			
			UFunctions::ServerReadyToStartMatch();

			InitCombos();

			CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&Thread), nullptr, NULL, nullptr);

			UFunctions::ConsoleLog(XOR(L"\n\nWelcome to Neonite++\nMade with ♥ By Kemo (@xkem0x on twitter)."));

			//ConnectServer();

			bIsInit = !bIsInit;
		}
	}
}
