#pragma once
#include <chrono>//We will use chrono for the elapsed time.

//Discord SDK Stuff.
#include "Frontend/util.h"

static bool gInit, gRPC = true;
DiscordRichPresence discordPresence;

void SetupDiscord()
{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("776056054818668544", &handlers, 1, "0");
}


static void UpdateDiscord()
{
	static int64_t StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "Using Neonite";
		discordPresence.details = "In Launcher";
		discordPresence.startTimestamp = StartTime;
		discordPresence.endTimestamp = NULL;
		discordPresence.largeImageKey = "big-image";
		discordPresence.largeImageText = "Neonite";
		discordPresence.instance = 1;

		Discord_UpdatePresence(&discordPresence);
	
}

void StartRPC()
{
	SetupDiscord();
	UpdateDiscord();
}
