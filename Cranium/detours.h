#pragma once
#include "hooks.h"

void* ProcessEventDetour(UObject* pObj, UObject* pFunc, void* pParams)
{
	if (pObj && pFunc)
	{
		auto nObj = GetObjectName(pObj);
		auto nFunc = GetObjectName(pFunc);

		/*
		if(nFunc.c_str() == L"BP_OnClicked" && nObj.c_str() == L"Button_Play")
		{
			//trigger
		}
		*/
		/*
		if (!wcsstr(nFunc.c_str(), L"Tick") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixEnvelope") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixSpectralAnalysis") &&
			!wcsstr(nFunc.c_str(), L"ReadyToEndMatch")
			)
		{
			printf("LogObject: %ws\nLogFunction: %ws\n", nObj.c_str(), nFunc.c_str());
		}
		*/
	}

	return ProcessEvent(pObj, pFunc, pParams);
}

POINT pMouse;
HWND hGame;

namespace GVPM
{
	float FOV = 52.0;
	FVector Camera(52.274170, 125912.695313, 89.249969);
	FRotator Rotation = { 0.870931, -88.071960, 0.008899};
}

bool bFreeCameraEnabled = true;

int GetViewPointDetour(void* pPlayer, FMinimalViewInfo* pViewInfo, BYTE stereoPass)
{
	auto CurrentViewPoint = GetViewPoint(pPlayer, pViewInfo, stereoPass);

	//TODO: proper input handling/hook
	if (GetAsyncKeyState(VK_ADD) == 0)
	{
		GVPM::FOV += 0.1;
	}

	if (GetAsyncKeyState(VK_SUBTRACT) == 0)
	{
		GVPM::FOV -= 0.1;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) == 0)
	{
		//Camera UP
		GVPM::Camera.Y += 0.5;
	}

	if (GetAsyncKeyState(VK_NUMPAD5) == 0)
	{
		//Camera DOWN
		GVPM::Camera.Y -= 0.5;
	}

	if (GetAsyncKeyState(VK_NUMPAD4) == 0)
	{
		//Camera FORWARD
		GVPM::Camera.X += 0.5;
	}

	if (GetAsyncKeyState(VK_NUMPAD6) == 0)
	{
		//Camera BACK
		GVPM::Camera.X -= 0.5;
	}

	if (GetAsyncKeyState(VK_NUMPAD9) == 0)
	{
		//Camera RIGHT
		GVPM::Camera.Z += 0.5;
	}

	if (GetAsyncKeyState(VK_NUMPAD7) == 0)
	{
		//Camera LEFT
		GVPM::Camera.Z -= 0.5;
	}

	if (GetAsyncKeyState(VK_UP) == 0)
	{
		//Look UP
		GVPM::Rotation.Pitch -= 0.1;
	}

	if (GetAsyncKeyState(VK_DOWN) == 0)
	{
		//Look DOWN
		GVPM::Rotation.Pitch += 0.1;
	}

	if (GetAsyncKeyState(VK_LEFT) == 0)
	{
		//Look LEFT
		GVPM::Rotation.Yaw += 0.1;
	}

	if (GetAsyncKeyState(VK_RIGHT) == 0)
	{
		//Look RIGHT
		GVPM::Rotation.Yaw -= 0.1;
	}

	/*
	hGame = FindWindow((L"UnrealWindow"), (L"Fortnite  "));

	if (GetCursorPos(&pMouse))
	{
		if (ScreenToClient(hGame, &pMouse) && bFreeCameraEnabled)
		{
			//3d maths go duuuuuuh
			//GVPM::Rotation.Yaw = atan2(pMouse.y, pMouse.x) * PI / 180.0f;
			//ShowCursor(BOOLEAN);
		}
	}
	*/
	
	pViewInfo->Location.X = GVPM::Camera.X;
	pViewInfo->Location.Y = GVPM::Camera.Y;
	pViewInfo->Location.Z = GVPM::Camera.Z;
	pViewInfo->Rotation.Pitch = GVPM::Rotation.Pitch;
	pViewInfo->Rotation.Yaw = GVPM::Rotation.Yaw;
	pViewInfo->Rotation.Roll = GVPM::Rotation.Roll;

	printf("\nPitch: %f\nYaw: %f", GVPM::Rotation.Pitch, GVPM::Rotation.Yaw);

	pViewInfo->FOV = GVPM::FOV;
	return CurrentViewPoint;
}
