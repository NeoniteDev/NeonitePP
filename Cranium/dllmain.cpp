#include "pch.h"
#include "curlhooks.h"
#include "hooks.h"
#include "gui/gui.h"

DWORD WINAPI ImguiThread(HMODULE hModule)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

void dllMain(HMODULE hModule)
{
#ifdef CONSOLE
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	//freopen_s(&fDummy, "ProcessEvent.log", "w", stdout);
#endif

#ifdef SSL_BYPASS
	Hooks::curl();
#endif


#ifdef HOOKS
	while (true)
	{
		if (isReady)
		{
			if (Hooks::Misc(gVersion) && Console::Unlock())
			{
				ImguiThread(hModule);
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
		dllMain(hModule);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}
	return TRUE;
}
