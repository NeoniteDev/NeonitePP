#include "pch.h"
#include "curlhooks.h"
#include "hooks.h"
#include "gui/gui.h"

bool WINAPI ImguiThread()
{
	while (true)
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, reinterpret_cast<void**>(&oPresent), hkPresent);
			break;
		}
	};
	return true;
}

void dllMain()
{
	
#ifdef CONSOLE
#ifndef PROD
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	//freopen_s(&fDummy, "ProcessEvent.log", "w", stdout);
#endif
	printf(XOR("[=]Hang tight, it's not broken the game is just loading lol.\nMade by kemo (@xkem0x on twitter, credit me owo)"));
#endif

#ifdef SSL_BYPASS
	Hooks::curl();
#endif


#ifdef HOOKS
	while (true)
	{
		if (isReady)
		{													//Deprecated
			if (Hooks::Misc(gVersion) && Console::Unlock()) //&& ForceSettings())
			{
				ImguiThread();
				break;
			}
		}
		Sleep(1000 / 30); //30 fps  
	}
}
#endif


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		dllMain();
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}
	return TRUE;
}
