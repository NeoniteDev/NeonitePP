#pragma once

namespace DefaultRuntimeOptions
{
#ifdef PROD
	inline std::string ini =
		R"([/Script/FortniteGame.FortRuntimeOptions]
bAllowInGameStore=false
bMOTDSameNewsForCreative=true
bForceBRMode=true
bEnableSavedLoadouts=false
bInviteUIDisabled=false
bIsOutOfSeasonMode=true
!DisabledTabsForOutOfSeason=ClearArray
+DisabledTabsForOutOfSeason=(TabName="Lobby",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="AthenaCompete",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="AthenaCareer",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="AthenaStore",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="CareerScreen",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="AthenaDirectAcquisition",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="BattlePass",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledTabsForOutOfSeason=(TabName="AthenaCustomize",TabState=EFortRuntimeOptionTabState::Hidden)
)";
#else
	inline std::string ini =
		R"([/Script/FortniteGame.FortRuntimeOptions]
bAllowInGameStore=false
bMOTDSameNewsForCreative=true
bForceBRMode=true
bEnableSavedLoadouts=false
bAllowAllReplays=true
)";
#endif


	inline std::string name = "DefaultRuntimeOptions.ini";
	inline std::string sha1 = "holder";
	inline std::string sha256 = "holder";
}
