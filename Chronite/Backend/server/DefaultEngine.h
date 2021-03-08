#pragma once

namespace DefaultEngine
{
	inline std::string ini =
	R"(
[ConsoleVariables]
FortMatchmakingV2.EnableContentBeacon=0
FortMatchmakingV2.ContentBeaconFailureCancelsMatchmaking=0

[Core.Log]
LogHttp=error
LogXmpp=error
LogBeacon=verbose
LogQos=error
LogOnline=verbose
LogOnlineCloud=verbose
LogOnlineGame=verbose
LogSourceControl=verbose
LogUAC=verbose
LogBattlEye=verbose
LogEasyAntiCheatServer=verbose
LogEasyAntiCheatClient=verbose
LogFortAnalytics=verbose
LogEpicCMS=verbose
LogNetPlayerMovement=verbose
LogDerivedDataCache=verbose
LogOnlineDataAssetDirectory=verbose
LogContentBeacon=verbose
LogBPSInstallerConfig=verbose

[PatchCheck]
ModuleName=FortnitePatchCheck
bCheckPlatformOSSForUpdate=false
bCheckOSSForUpdate=false

[XMPP]
bEnableWebsockets=false
	
[/Script/Qos.QosRegionManager]
!RegionDefinitions=ClearArray
+RegionDefinitions=(DisplayName=NSLOCTEXT("MMRegion", "NPP", "NPP"), RegionId="NPP", bEnabled=true, bVisible=true, bAutoAssignable=true)
)";

	inline std::string name = "DefaultEngine.ini";
	inline std::string sha1 = "C8C23C183E44C2AADD3D7DE90CAF451DDD2D6E82";
	inline std::string sha256 = "223C13EDE8ECF4C4EF8702B8FCC5A247C27A98F399DA03054D3F8F119C0F0D4A";
}
