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
bEnableClientSettingsSaveToCloud=false
DefaultFlagRegionId=Neonite
)";

	inline std::string name = "DefaultRuntimeOptions.ini";
	inline std::string sha1 = "holder";
	inline std::string sha256 = "holder";
}
