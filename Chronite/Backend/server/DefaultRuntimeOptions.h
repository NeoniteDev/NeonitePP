#pragma once

namespace DefaultRuntimeOptions
{
	inline std::string ini =
	R"([/Script/FortniteGame.FortRuntimeOptions]
!DisabledFrontendNavigationTabs=ClearArray
+DisabledFrontendNavigationTabs=(TabName="AthenaCompete",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledFrontendNavigationTabs=(TabName="CareerScreen",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledFrontendNavigationTabs=(TabName="ItemShopSelector",TabState=EFortRuntimeOptionTabState::Hidden)
+DisabledFrontendNavigationTabs=(TabName="StoreMain_Root",TabState=EFortRuntimeOptionTabState::Hidden)
bAllowInGameStore=false
bMOTDSameNewsForCreative=true
bForceBRMode=true
bEnableSavedLoadouts=false
bAllowAllReplays=true
)";

	inline std::string name = "DefaultRuntimeOptions.ini";
	inline std::string sha1 = "521842f4ccda42136d099c8a288b7c3b20172e5e";
	inline std::string sha256 = "05290d7c6e4cb94b0a28bb9547ad21a4cea79ca986c7d911bb159fb61421d96d";
}
