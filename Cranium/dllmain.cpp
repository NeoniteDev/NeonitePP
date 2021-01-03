#include "pch.h"
#include "curlhooks.h"
#include "hooks.h"
#include "util.h"

void dllMain()
{
#ifdef CONSOLE
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

#ifdef SSL_BYPASS
	Hooks::curl();
#endif

#ifdef HOOKS
	while (true)
	{
		if (isReady)
		{
			if (Hooks::Misc() && DumpIDs() && Console::Unlock()) //IKR THIS CAN CAUSE A MEMORY LEAK BUT I FUCKING MERGED THE WRONG FILE AND I DONT REMEBER WHAT I DID
			{
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
