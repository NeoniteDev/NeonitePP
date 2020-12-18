#include "pch.h"
#include "curlhooks.h"
#include "enums.h"
#include "veh.h"
#include "util.h"
#include "structs.h"

#define USE_CONSOLE

void Main()
{
#ifdef USE_CONSOLE
	AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "w", stdout);
#endif

	//CURL Detour
	const auto CurlEasyAdd = Util::FindPattern(Patterns::CurlEasySetOpt, Masks::CurlEasySetOpt);
	VALIDATE_ADDRESS(CurlEasyAdd, "Failed to find Curl Easy Address.");

	const auto CurlSetAdd = Util::FindPattern(Patterns::CurlSetOpt, Masks::CurlSetOpt);
	VALIDATE_ADDRESS(CurlSetAdd, "Failed to find Curl SetOpt Address.");

	curl_easy_setopt = decltype(curl_easy_setopt)(CurlEasyAdd);
	curl_setopt = decltype(curl_setopt)(CurlSetAdd);

	VEH::EnableHook(curl_easy_setopt, curl_easy_setopt_detour);

	//Console Spawner
	bool cIsSpawned = false;
	while (true)
	{
		if (GetAsyncKeyState(VK_OEM_3) && !cIsSpawned)
		{
			const auto GEngineAdd = Util::FindPattern(Patterns::GEngine, Masks::GEngine);
			VALIDATE_ADDRESS(GEngineAdd, "Failed to find GEngine Address.");

			GEngine = *(UEngine**)(GEngineAdd + 22 + *(int32_t*)(GEngineAdd + 18));

			const auto SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
			VALIDATE_ADDRESS(SCOIAdd, "Failed to find SCOI Address.");

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
			cIsSpawned = true;
			break;
		}
		Sleep(300);
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Main();
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}
	return TRUE;
}
