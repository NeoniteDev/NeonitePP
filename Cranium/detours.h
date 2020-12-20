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

		if (!wcsstr(nFunc.c_str(), L"Tick") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixEnvelope") &&
			!wcsstr(nFunc.c_str(), L"OnSubmixSpectralAnalysis") &&
			!wcsstr(nFunc.c_str(), L"ReadyToEndMatch")
			)
		{
			printf("LogObject: %ws\nLogFunction: %ws\n", nObj.c_str(), nFunc.c_str());
		}
	}

	return ProcessEvent(pObj, pFunc, pParams);
}
