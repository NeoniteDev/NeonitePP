#pragma once
#include "ue4.h"
#include "mods.h"

inline bool bIsDebugCamera = false;

void* ProcessEventDetour(UObject* pObj, UObject* pFunc, void* pParams)
{
	if (pObj && pFunc)
	{
		auto nObj = GetObjectFirstName(pObj);
		auto nFunc = GetObjectFirstName(pFunc);

		//Open game mode base on play click
		if (nFunc == L"BP_OnClicked" && nObj == L"Button_Play")
		{
			UFunctions::Travel(APOLLO_PAPAYA_BASE);
		}

		//Destroy all HLODs after the loading screen.a
		//TODO: better function as this will crash on back to lobby.
		if (nFunc == L"DynamicHandleLoadingScreenVisibilityChanged" && nObj == L"AthenaLobby")
		{
			Console::CheatManager();
			UFunctions::DestroyAllHLODs();
		}

		if (nFunc == L"ToggleDebugCamera" && nObj.starts_with(L"FortCheatManager_"))
		{
			bIsDebugCamera = !bIsDebugCamera;
		}

		//LOG ARG
		/*
		if (nFunc == L"DestroyAll")
		{
			auto className = static_cast<UCheatManager_DestroyAll_Params*>(pParams)->Class;
			std::wstring classNameW = GetObjectFullName(className);
			printf("\n\n\nCLASSNAME: %ls", classNameW.c_str());
		}
		*/

		if (!wcsstr(nFunc.c_str(), L"EvaluateGraphExposedInputs") &&
			!wcsstr(nFunc.c_str(), L"Tick") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixEnvelope") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixSpectralAnalysis") &&
			!wcsstr(nFunc.c_str(), L"OnMouse") &&
			!wcsstr(nFunc.c_str(), L"BlueprintUpdateAnimation") &&
			!wcsstr(nFunc.c_str(), L"BlueprintPostEvaluateAnimation") &&
			!wcsstr(nFunc.c_str(), L"BlueprintModifyCamera") &&
			!wcsstr(nFunc.c_str(), L"BlueprintModifyPostProcess") &&
			!wcsstr(nFunc.c_str(), L"Loop Animation Curve") &&
			!wcsstr(nFunc.c_str(), L"UpdateTime") &&
			!wcsstr(nFunc.c_str(), L"ReadyToEndMatch"))
		{
			printf("LogObject: %ws\nLogFunction: %ws\n", nObj.c_str(), nFunc.c_str());
		}
	}

	return ProcessEvent(pObj, pFunc, pParams);
}

namespace CameraHook
{
	float Speed = 0.1;
	float FOV = 52.0;
	FVector Camera(52.274170, 125912.695313, 89.249969);
	FRotator Rotation = {0.870931, -88.071960, 0.008899};
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
