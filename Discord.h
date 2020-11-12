#pragma once
#include <chrono>//We will use chrono for the elapsed time.

//Discord SDK Stuff.
#include "Frontend/util.h"
#include "includes/DiscordSDK/discord_rpc.h"
#include "includes/DiscordSDK/discord_register.h"

static bool gInit, gRPC = true;

void SetupDiscord()
{
	if (bUseRPC) {
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("776056054818668544", &handlers, 1, "0");
	} else {
		Discord_Shutdown();
	}
}


static void UpdateDiscord()
{
	static int64_t StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	if (bUseRPC) {

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "Using Neonite";
		discordPresence.details = "In Launcher";
		discordPresence.startTimestamp = StartTime;
		discordPresence.endTimestamp = NULL;
		discordPresence.largeImageKey = "big-image";
		discordPresence.largeImageText = "Neonite";
		discordPresence.instance = 1;

		Discord_UpdatePresence(&discordPresence);

	} else {
		Discord_ClearPresence();
	}
	
}

void StartRPC()
{
	SetupDiscord();
	UpdateDiscord();
}