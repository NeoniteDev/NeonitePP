#pragma once

struct UObject;

struct UClass;

struct UConsole;

struct UGameViewportClient
{
	char unknown1[0x40];
	UConsole* ViewportConsole;
};

struct UEngine
{
	char unknown1[0xF8];
	UClass* ConsoleClass;
	char unknown2[0x688];
	UGameViewportClient* GameViewportClient;
};
