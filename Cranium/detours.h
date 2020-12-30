#pragma once
#include "hooks.h"

void* ProcessEventDetour(UObject* pObj, UObject* pFunc, void* pParams)
{
	if (pObj && pFunc)
	{
		auto nObj = GetObjectFirstName(pObj);
		auto nFunc = GetObjectFirstName(pFunc);

		if (nFunc == L"Summon")
		{
			auto className = static_cast<UCheatManager_Summon_Params*>(pParams)->ClassName;
			std::wstring classNameW = className.ToString();
			printf("\n\n\nCLASSNAME: %ls", classNameW.c_str());
		}


		if (!wcsstr(nFunc.c_str(), L"Tick") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixEnvelope") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixSpectralAnalysis") &&
			!wcsstr(nFunc.c_str(), L"OnMouse") &&
			!wcsstr(nFunc.c_str(), L"ReadyToEndMatch"))
		{
			//printf("LogObject: %ws\nLogFunction: %ws\n", nObj.c_str(), nFunc.c_str());
		}
	}

	return ProcessEvent(pObj, pFunc, pParams);
}

POINT pMouse;
HWND hGame;

namespace GVPM
{
	float Speed = 0.1;
	float FOV = 52.0;
	FVector Camera(52.274170, 125912.695313, 89.249969);
	FRotator Rotation = {0.870931, -88.071960, 0.008899};
}

int GetViewPointDetour(void* pPlayer, FMinimalViewInfo* pViewInfo, BYTE stereoPass)
{
	auto CurrentViewPoint = GetViewPoint(pPlayer, pViewInfo, stereoPass);

	//if capslock is off
	if (GetKeyState(VK_CAPITAL) != 0)
	{
		//TODO: proper input handling/hook

		if (GetAsyncKeyState(VK_OEM_PLUS) == 0)
		{
			GVPM::FOV += GVPM::Speed;
		}

		if (GetAsyncKeyState(VK_OEM_MINUS) == 0)
		{
			GVPM::FOV -= GVPM::Speed;
		}

		if (GetAsyncKeyState(0x55) == 0)
		{
			//Camera UP
			GVPM::Camera.Y += GVPM::Speed;
		}

		if (GetAsyncKeyState(0x4A) == 0)
		{
			//Camera DOWN
			GVPM::Camera.Y -= GVPM::Speed;
		}

		if (GetAsyncKeyState(0x48) == 0)
		{
			//Camera FORWARD
			GVPM::Camera.X += GVPM::Speed;
		}

		if (GetAsyncKeyState(0x4B) == 0)
		{
			//Camera BACK
			GVPM::Camera.X -= GVPM::Speed;
		}

		if (GetAsyncKeyState(0x49) == 0)
		{
			//Camera RIGHT
			GVPM::Camera.Z += GVPM::Speed;
		}

		if (GetAsyncKeyState(0x59) == 0)
		{
			//Camera LEFT
			GVPM::Camera.Z -= GVPM::Speed;
		}

		if (GetAsyncKeyState(VK_UP) == 0)
		{
			//Look UP
			GVPM::Rotation.Pitch -= GVPM::Speed;
		}

		if (GetAsyncKeyState(VK_DOWN) == 0)
		{
			//Look DOWN
			GVPM::Rotation.Pitch += GVPM::Speed;
		}

		if (GetAsyncKeyState(VK_LEFT) == 0)
		{
			//Look LEFT
			GVPM::Rotation.Yaw += GVPM::Speed;
		}

		if (GetAsyncKeyState(VK_RIGHT) == 0)
		{
			//Look RIGHT
			GVPM::Rotation.Yaw -= GVPM::Speed;
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

		pViewInfo->FOV = GVPM::FOV;

		//pViewInfo->ProjectionMode = ECameraProjectionMode::Orthographic; //i hate you armisto
	}
	else
	{
		GVPM::FOV = 52.0;
		GVPM::Camera.X = 52.274170;
		GVPM::Camera.Y = 125912.695313;
		GVPM::Camera.Z = 89.249969;
		GVPM::Rotation.Pitch = 0.870931;
		GVPM::Rotation.Yaw = -88.071960;
		GVPM::Rotation.Roll = 0.008899;
	}

	return CurrentViewPoint;
}
