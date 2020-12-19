#include "pch.h"
#include "curlhooks.h"
#include "enums.h"
#include "hooks.h"
#include "veh.h"
#include "util.h"
#include "structs.h"

void dllMain()
{
#ifdef CONSOLE
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	if (MH_Initialize() != MH_OK)
	{
		MessageBoxA(0, "Failed to initialize min-hook, terminating the thread.", "Cranium", MB_OK);
		FreeLibraryAndExitThread(GetModuleHandle(NULL), 0);
	}
	
	Hooks::init();

	//CURL Detour
#ifdef SSL_BYPASS

	CurlEasySetOpt = decltype(CurlEasySetOpt)(CurlEasyAdd);
	CurlSetOpt = decltype(CurlSetOpt)(CurlSetAdd);

	VEH::EnableHook(CurlEasySetOpt, CurlEasySetOptDetour);

#endif

	bool isUnlocked = false;
	bool isHooked = false;
	while (true)
	{
		//Process Event Hooking
#ifdef PROCESS_EVENT
		if (GetAsyncKeyState(VK_F10) && !isHooked)
		{
			ProcessEvent = decltype(ProcessEvent)(ProcessEventAdd);
			
			MH_CreateHook((void*)ProcessEventAdd, ProcessEventDetour, (void**)&ProcessEvent);
			MH_EnableHook((void*)ProcessEventAdd);
			
			isHooked = true;
		}
#endif

#ifdef UNLOCK_CONSOLE
		//Console Spawner
		if (GetAsyncKeyState(VK_OEM_3) && !isUnlocked)
		{
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
			isUnlocked = true;
		}
#endif

#if(defined UNLOCK_CONSOLE && defined PROCESS_EVENT)
		if (isUnlocked && isHooked) break;
#endif

		Sleep(300);
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
