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
HWND hGame = GetActiveWindow();

namespace GVPM
{
	float FOV = 52.0;
	FVector Camera(52.274170, 125912.695313, 89.249969);
}

int GetViewPointDetour(void* pPlayer, FMinimalViewInfo* pViewInfo, BYTE stereoPass)
{
	auto CurrentViewPoint = GetViewPoint(pPlayer, pViewInfo, stereoPass);

	//TODO: proper input handling/hook
	if(GetAsyncKeyState(VK_ADD) == 0)
	{
		GVPM::FOV += 1;
	}

	if(GetAsyncKeyState(VK_SUBTRACT) == 0)
	{
		GVPM::FOV -= 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) == 0)
	{
		//Camera UP
		GVPM::Camera.Y += 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD5) == 0)
	{
		//Camera DOWN
		GVPM::Camera.Y -= 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD4) == 0)
	{
		//Camera FORWARD
		GVPM::Camera.X += 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD6) == 0)
	{
		//Camera BACK
		GVPM::Camera.X -= 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD9) == 0)
	{
		//Camera RIGHT
		GVPM::Camera.Z += 1;
	}

	if (GetAsyncKeyState(VK_NUMPAD7) == 0)
	{
		//Camera LEFT
		GVPM::Camera.Z -= 1;
	}

	if(GetCursorPos(&pMouse))
	{
		if(ScreenToClient(hGame, &pMouse))
		{
			printf("X: %i\nY: %i\n", pMouse.x, pMouse.y);
		}
	}

	//ShowCursor(BOOLEAN);

	pViewInfo->Location.X = GVPM::Camera.X;
	pViewInfo->Location.Y = GVPM::Camera.Y;
	pViewInfo->Location.Z = GVPM::Camera.Z;
	
	pViewInfo->FOV = GVPM::FOV;
	return CurrentViewPoint;
}
