#pragma once
#include "ue4.h"
#include "mods.h"

//#define LOGGING

inline bool bIsDebugCamera = false;

inline void* ProcessEventDetour(UObject* pObj, UObject* pFunc, void* pParams)
{
	const auto nObj = GetObjectFirstName(pObj);
	const auto nFunc = GetObjectFirstName(pFunc);

	//If the game requested matchmaking we open the game mode
	if (gUrl.find(XOR("matchmakingservice")) != std::string::npos)
	{
		Neoroyale::start();
		gUrl.clear();
	}

	if (wcsstr(nFunc.c_str(), XOR(L"ReadyToStartMatch")) && Neoroyale::bIsStarted && !Neoroyale::bIsInit)
	{
		Neoroyale::init();
	}

	//Destroy all HLODs after the loading screen.
	if (wcsstr(nFunc.c_str(), XOR(L"DynamicHandleLoadingScreenVisibilityChanged")) && wcsstr(nObj.c_str(), XOR(L"AthenaLobby")))
	{
		if (bIsDebugCamera) bIsDebugCamera = !bIsDebugCamera;
		Console::CheatManager();
		UFunctions::DestroyAllHLODs();
	}

	//Toggle our debug camera on "toggledebugcamera" command.
	if (wcsstr(nFunc.c_str(), XOR(L"ToggleDebugCamera")) && nObj.starts_with(XOR(L"FortCheatManager_")))
	{
		bIsDebugCamera = !bIsDebugCamera;
	}

	if (wcsstr(nFunc.c_str(), XOR(L"CheatScript")))
	{
		FString ScriptNameF = static_cast<UCheatManager_CheatScript_Params*>(pParams)->ScriptName;
		if (ScriptNameF.IsValid())
		{
			std::wstring ScriptNameW = ScriptNameF.ToWString();
			if (wcsstr(ScriptNameW.c_str(), XOR(L"help")))
			{
				MessageBoxA(nullptr,
				            XOR(
					            "dump: Dumps all fornite GObjects\n\ndumpbps: Dumps all fornite Blueprints\n\nevent: Triggers whatever event in your version."),
				            XOR("Cranium CheatScript Commands"), MB_OK);
			}
			else if (ScriptNameW == XOR(L"dumpbps"))
			{
				DumpBPs();
			}
			else if (ScriptNameW == XOR(L"dump"))
			{
				DumpGObjects();
			}
			else if (ScriptNameW == XOR(L"event"))
			{
				if (gVersion == XOR("14.60"))
				{
					UFunctions::Play(GALACTUS_EVENT);
				}
				else if (gVersion == XOR("12.41"))
				{
					UFunctions::Play(JERKY_EVENT);
				}
				else if (gVersion == XOR("12.61"))
				{
					UFunctions::Play(DEVICE_EVENT);
				}
				else
				{
					MessageBoxA(nullptr, XOR("Sorry the version you are using doesn't have any event we support."), XOR("Neonite++"), MB_OK);
				}
			}
		}
	}

#ifdef LOGGING
	//Logging
	if (!wcsstr(nFunc.c_str(), L"EvaluateGraphExposedInputs") &&
		!wcsstr(nFunc.c_str(), L"Tick") &&
		!wcsstr(nFunc.c_str(), L"OnSubmixEnvelope") &&
		!wcsstr(nFunc.c_str(), L"OnSubmixSpectralAnalysis") &&
		!wcsstr(nFunc.c_str(), L"OnMouse") &&
		!wcsstr(nFunc.c_str(), L"Pulse") &&
		!wcsstr(nFunc.c_str(), L"BlueprintUpdateAnimation") &&
		!wcsstr(nFunc.c_str(), L"BlueprintPostEvaluateAnimation") &&
		!wcsstr(nFunc.c_str(), L"BlueprintModifyCamera") &&
		!wcsstr(nFunc.c_str(), L"BlueprintModifyPostProcess") &&
		!wcsstr(nFunc.c_str(), L"Loop Animation Curve") &&
		!wcsstr(nFunc.c_str(), L"UpdateTime") &&
		!wcsstr(nFunc.c_str(), L"GetMutatorByClass") &&
		!wcsstr(nFunc.c_str(), L"UpdatePreviousPositionAndVelocity") &&
		!wcsstr(nFunc.c_str(), L"ReadyToEndMatch"))
	{
		printf("[Object]: %ws [Function]: %ws\n", nObj.c_str(), nFunc.c_str());
	}
#endif

	return ProcessEvent(pObj, pFunc, pParams);
}

namespace CameraHook
{
	inline float Speed = 0.1;
	inline float FOV = 52.0;
	inline FVector Camera(52.274170, 125912.695313, 89.249969);
	inline FRotator Rotation = {0.870931, -88.071960, 0.008899};
}

inline int GetViewPointDetour(void* pPlayer, FMinimalViewInfo* pViewInfo, BYTE stereoPass)
{
	const auto CurrentViewPoint = GetViewPoint(pPlayer, pViewInfo, stereoPass);

	if (bIsDebugCamera)
	{
		//fov increase and decrease
		if (GetAsyncKeyState(VK_OEM_PLUS) == 0) CameraHook::FOV += CameraHook::Speed;

		if (GetAsyncKeyState(VK_OEM_MINUS) == 0) CameraHook::FOV -= CameraHook::Speed;

		//froward and backward left and right
		if (GetAsyncKeyState(0x57) == 0) CameraHook::Camera.Y += CameraHook::Speed;

		if (GetAsyncKeyState(0x53) == 0) CameraHook::Camera.Y -= CameraHook::Speed;

		if (GetAsyncKeyState(0x41) == 0) CameraHook::Camera.X += CameraHook::Speed;

		if (GetAsyncKeyState(0x44) == 0) CameraHook::Camera.X -= CameraHook::Speed;

		//up and down
		if (GetAsyncKeyState(0x45) == 0) CameraHook::Camera.Z += CameraHook::Speed;

		if (GetAsyncKeyState(0x51) == 0) CameraHook::Camera.Z -= CameraHook::Speed;


		//looking around
		if (GetAsyncKeyState(VK_UP) == 0) CameraHook::Rotation.Pitch -= CameraHook::Speed;

		if (GetAsyncKeyState(VK_DOWN) == 0) CameraHook::Rotation.Pitch += CameraHook::Speed;

		if (GetAsyncKeyState(VK_LEFT) == 0) CameraHook::Rotation.Yaw += CameraHook::Speed;

		if (GetAsyncKeyState(VK_RIGHT) == 0) CameraHook::Rotation.Yaw -= CameraHook::Speed;

		//assign our hooked variables
		pViewInfo->Location.X = CameraHook::Camera.X;
		pViewInfo->Location.Y = CameraHook::Camera.Y;
		pViewInfo->Location.Z = CameraHook::Camera.Z;

		pViewInfo->Rotation.Pitch = CameraHook::Rotation.Pitch;
		pViewInfo->Rotation.Yaw = CameraHook::Rotation.Yaw;
		pViewInfo->Rotation.Roll = CameraHook::Rotation.Roll;

		pViewInfo->FOV = CameraHook::FOV;
	}

	return CurrentViewPoint;
}
