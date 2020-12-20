#include "pch.h"
#include "curlhooks.h"
#include "enums.h"
#include "hooks.h"
#include "veh.h"
#include "util.h"
#include "structs.h"

void dllMain()
{
	FILE* fDummy;
	AllocConsole();
#ifdef CONSOLE
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	freopen_s(&fDummy, "cranium.log", "w", stdout);
#endif
	
	//CURL Detour
#ifdef SSL_BYPASS
	Hooks::curl();

	CurlEasySetOpt = decltype(CurlEasySetOpt)(CurlEasyAdd);
	CurlSetOpt = decltype(CurlSetOpt)(CurlSetAdd);

	VEH::EnableHook(CurlEasySetOpt, CurlEasySetOptDetour);

#endif

	while(true)
	{

		if (isReady)
		{
			Hooks::init();
			
#ifdef HOOKS

			ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);

			MH_CreateHook((void*)ProcessEventAdd, ProcessEventDetour, (void**)&ProcessEvent);
			MH_EnableHook((void*)ProcessEventAdd);

#endif
			
#ifdef UNLOCK_CONSOLE
			
			GEngine = *(UEngine**)(GEngineAdd + 22 + *(int32_t*)(GEngineAdd + 18));

			StaticConstructObject_Internal = (f_StaticConstructObject_Internal)(SCOIAdd);

			UConsole* Console = reinterpret_cast<UConsole*>(StaticConstructObject_Internal(
				GEngine->ConsoleClass,
				reinterpret_cast<UObject*>(GEngine->GameViewportClient),
				nullptr,
				RF_NoFlags,
				None,
				nullptr,
				false,
				nullptr,
				false
			));

			GEngine->GameViewportClient->ViewportConsole = Console;
			
#endif
	
			break;
		}
		Sleep(1000 / 30); //30 fps 
	}
}

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
