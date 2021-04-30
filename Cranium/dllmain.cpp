/**
 * Copyright (c) 2020-2021 Kareem Olim (Kemo)
 * All Rights Reserved. Licensed under the Neo License
 * https://neonite.dev/LICENSE.html
 */

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

void WINAPI dllMain()
{
#ifdef CONSOLE
//#ifndef PROD
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	//freopen_s(&fDummy, "ProcessEvent.log", "w", stdout);
//#endif
	printf(XOR("[=]Scuffed build for latest, Made by kemo (@xkem0x on twitter)"));
#endif

#ifdef SSL_BYPASS
	Hooks::Init();
#endif


#ifdef HOOKS
	while (true)
	{
		if (isReady)
		{
			 //Deprecated
			if (Hooks::Misc(gVersion)) //&& Console::Unlock()) //&& ForceSettings())
			{
				ImguiThread();
				break;
			}
		}
		else if (GetAsyncKeyState(VK_F3))
		{
			if (Hooks::Misc(99.99f))
			{
				//ImguiThread();
				break;
			}
		}
		Sleep(1000 / 30); //30 fps  
	}
#endif
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
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
