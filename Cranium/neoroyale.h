#pragma once
#include <vector>

#include "mods.h"

inline std::vector<std::wstring> gWeapons;
inline std::vector<std::wstring> gBlueprints;

namespace Neoroyale
{
	inline bool bIsInit;
	inline bool bIsStarted;
	inline bool bIsPlayerInit;
	inline bool bHasJumped;
	inline bool bHasJumpedFromBus;
	inline bool bHasShowedPickaxe;
	inline Player NeoPlayer;

	inline void start(const wchar_t* MapToPlayOn)
	{
		UFunctions::Travel(MapToPlayOn);
		bIsStarted = !bIsStarted;
	}


	inline void InitCombos()
	{
		for (auto i = 0x0; i < GObjs->NumElements; ++i)
		{
			const auto object = GObjs->GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}

			if (!Util::IsBadReadPtr(object))
			{
				auto objectFullName = GetObjectFullName(object);
				auto objectFirstName = GetObjectFirstName(object);

				if (objectFullName.starts_with(L"AthenaGadget") || objectFirstName.starts_with(L"WID_"))
				{
					gWeapons.push_back(objectFirstName);
				}
				else if (objectFirstName.ends_with(L"_C"))
				{
					gBlueprints.push_back(objectFirstName);
				}
			}
		}
	}

	inline void gametick()
	{
		//TODO: better keyboard hook
		if (NeoPlayer.Pawn && GetAsyncKeyState(VK_SPACE))
		{
			if (!bHasJumped)
			{
				bHasJumped = !bHasJumped;
				if (!NeoPlayer.IsInAircraft())
				{
					//Stopping emotes
					NeoPlayer.StopMontageIfEmote();

					//Glide
					if (NeoPlayer.IsSkydiving() && !NeoPlayer.IsParachuteOpen() && !NeoPlayer.IsParachuteForcedOpen())
					{
						NeoPlayer.ForceOpenParachute();
					}

						//Skydive
					else if (NeoPlayer.IsSkydiving() && NeoPlayer.IsParachuteOpen() && !NeoPlayer.IsParachuteForcedOpen())
					{
						if (!bHasJumpedFromBus)
						{
							const auto currentLocation = NeoPlayer.GetLocation();
							UFunctions::TeleportToCoords(currentLocation.X, currentLocation.Y, currentLocation.Z);
							bHasJumpedFromBus = !bHasJumpedFromBus;
						}
						NeoPlayer.Skydive();
					}

						//Jump
					else if (!NeoPlayer.IsJumpProvidingForce())
					{
						NeoPlayer.Jump();
					}
				}
			}
		}
		else bHasJumped = false;


		if (NeoPlayer.Pawn && GetAsyncKeyState(0x31) /* 1 key */)
		{
			if (!bHasShowedPickaxe)
			{
				bHasShowedPickaxe = !bHasShowedPickaxe;
				NeoPlayer.ShowPickaxe();
			}
		}
		else bHasShowedPickaxe = false;


		if (NeoPlayer.Pawn && GetAsyncKeyState(VK_F3))
		{
			UFunctions::Travel(FRONTEND);
			bIsStarted = false;
			bIsInit = false;
			NeoPlayer.Pawn = nullptr;
		}
	}

	auto Respawn()
	{
		if (NeoPlayer.Pawn)
		{
			UFunctions::DestoryActor(NeoPlayer.Pawn);

			NeoPlayer.Summon(L"PlayerPawn_Athena_C");
			NeoPlayer.Pawn = ObjectFinder::FindActor(L"PlayerPawn_Athena_C");

			if (NeoPlayer.Pawn)
			{
				NeoPlayer.Possess();
				NeoPlayer.ShowSkin();
				NeoPlayer.UpdateAnimInstance();
			}
		}
	}

	inline void init()
	{
		Console::CheatManager();

		UFunctions::DestroyAllHLODs();

		NeoPlayer.Summon(L"PlayerPawn_Athena_C");

		NeoPlayer.Pawn = ObjectFinder::FindActor(L"PlayerPawn_Athena_C");

		if (NeoPlayer.Pawn)
		{
			NeoPlayer.Possess();

			NeoPlayer.ShowSkin();

			NeoPlayer.ShowPickaxe();

			NeoPlayer.ToggleInfiniteAmmo();

			//LOL
			NeoPlayer.ExecuteConsoleCommand(L"god");

			//NeoPlayer.SetSkeletalMesh();

			const auto PlaylistName = GetObjectFirstName(gPlaylist);

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
			else
			{
				UFunctions::ConsoleLog(XOR(L"Sorry the version you are using doesn't have any event we support."));
			}

			UFunctions::StartMatch();

			UFunctions::ServerReadyToStartMatch();

			InitCombos();

			bIsInit = !bIsInit;
		}
	}
}
