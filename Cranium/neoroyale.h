#pragma once
#include "mods.h"

namespace Neoroyale
{
	inline bool bIsInit;
	inline bool bIsStarted;
	inline bool bHasJumped;
	inline bool bHasJumpedFromBus;
	inline Pawn* PlayerPawn;

	inline void start(const wchar_t* MapToPlayOn)
	{
		UFunctions::Travel(MapToPlayOn);
		bIsStarted = !bIsStarted;
	}

	inline auto Respawn()
	{
		if (PlayerPawn)
		{
			auto oldPawn = ObjectFinder::FindActor(L"PlayerPawn_Athena_C");
			if (oldPawn)
			{
				UFunctions::DestoryActor(oldPawn);
			}
			UFunctions::Summon(L"PlayerPawn_Athena_C");
			PlayerPawn = reinterpret_cast<Pawn*>(ObjectFinder::FindActor(L"PlayerPawn_Athena_C"));

			if (PlayerPawn)
			{
				PlayerPawn->Possess();
				PlayerPawn->ShowSkin();
				PlayerPawn->ShowPickaxe();
			}
		}
	}

	inline void gametick()
	{
		if (PlayerPawn && GetAsyncKeyState(VK_SPACE))
		{
			if (!bHasJumped)
			{
				bHasJumped = !bHasJumped;
				if (!PlayerPawn->IsInAircraft())
				{
					// Glide
					if (PlayerPawn->IsSkydiving() && !PlayerPawn->IsParachuteOpen() && !PlayerPawn->IsParachuteForcedOpen())
					{
						PlayerPawn->ForceOpenParachute();
					}

						// Skydive
					else if (PlayerPawn->IsSkydiving() && PlayerPawn->IsParachuteOpen() && !PlayerPawn->IsParachuteForcedOpen())
					{
						if (!bHasJumpedFromBus)
						{
							const auto currentLocation = PlayerPawn->GetLocation();
							UFunctions::TeleportToCoords(currentLocation.X, currentLocation.Y, currentLocation.Z);
							bHasJumpedFromBus = !bHasJumpedFromBus;
						}
						PlayerPawn->Skydive();
					}

						// Jump
					else if (!PlayerPawn->IsJumpProvidingForce())
					{
						PlayerPawn->Jump();
					}
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
		}
	}

	inline void init()
	{
		Console::CheatManager();

		UFunctions::DestroyAllHLODs();

		UFunctions::Summon(L"PlayerPawn_Athena_C");

		PlayerPawn = reinterpret_cast<Pawn*>(ObjectFinder::FindActor(L"PlayerPawn_Athena_C"));
		auto PlaylistName = GetObjectFirstName(gPlaylist);

		if (PlayerPawn)
		{
			PlayerPawn->Possess();

			//PlayerPawn->SetSkeletalMesh();

			PlayerPawn->ShowSkin();

			PlayerPawn->ShowPickaxe();

			PlayerPawn->ToggleInfiniteAmmo();

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
		}

		bIsInit = !bIsInit;
	}
}
