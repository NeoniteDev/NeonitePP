#pragma once

namespace DefaultEngine
{
	inline std::string ini =
	
R"(
[ConsoleVariables]
FortMatchmakingV2.EnableContentBeacon=0
FortMatchmakingV2.ContentBeaconFailureCancelsMatchmaking=0


[OnlineTracing]
bEnabled=true

[ConsoleVariables]
Store.EnableCatabaScreen=0
Store.EnableCatabaHighlights=0

[Core.Log]
LogHttp=all
LogXmpp=all
LogBeacon=all
LogQos=all
LogOnline=all
LogOnlineCloud=all
LogOnlineGame=all
LogOnlineParty=all
LogOnlinePresence=all
LogParty=all
LogOnlineChat=all
LogGarbage=all
LogTemp=all
LogSourceControl=all
LogLootTables=all
LogMatchmakingServiceClient=all
LogMatchmakingServiceDedicatedServer=all
LogUAC=all
LogBattlEye=all
LogEasyAntiCheatServer=all
LogEasyAntiCheatClient=all
LogFortAnalytics=all
LogVoiceEngine=Error
LogEpicCMS=all
LogNetPlayerMovement=all

[PatchCheck]
ModuleName=FortnitePatchCheck
bCheckPlatformOSSForUpdate=false
bCheckOSSForUpdate=false

[/Script/Qos.QosRegionManager]
!RegionDefinitions=ClearArray
+RegionDefinitions=(DisplayName=NSLOCTEXT("MMRegion", "Neonite", "Neonite"), RegionId="NAW", bEnabled=true, bVisible=true, bAutoAssignable=true)
)";

	inline std::string name = "DefaultEngine.ini";
	inline std::string sha1 = "1c29cbf4bc9f5b1b10cac0114122362601458256";
	inline std::string sha256 = "0fafc558d86c1c9155b8208c5d8972437a219131640d35931e78c0e9b095c371";
}
