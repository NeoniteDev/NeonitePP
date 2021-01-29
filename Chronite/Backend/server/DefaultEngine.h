#pragma once

namespace DefaultEngine
{
	inline std::string ini =
	R"(
[ConsoleVariables]
Store.EnableCatabaScreen=0
Store.EnableCatabaHighlights=0
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

[OnlineSubsystemMcp.Xmpp]
bUseSSL=false
ServerPort=443

[OnlineSubsystemMcp.Xmpp Prod]
bUseSSL=false
ServerAddr="ws://xmpp-service-prod.ol.epicgames.com"
Domain=prod.ol.epicgames.com
	
[/Script/Qos.QosRegionManager]
!RegionDefinitions=ClearArray
+RegionDefinitions=(DisplayName=NSLOCTEXT("MMRegion", "Neonite", "Neonite"), RegionId="EU", bEnabled=true, bVisible=true, bAutoAssignable=true)
)";

	inline std::string name = "DefaultEngine.ini";
	inline std::string sha1 = "C8C23C183E44C2AADD3D7DE90CAF451DDD2D6E82";
	inline std::string sha256 = "223C13EDE8ECF4C4EF8702B8FCC5A247C27A98F399DA03054D3F8F119C0F0D4A";
}
