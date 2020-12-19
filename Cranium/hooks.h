#pragma once
#include "pch.h"
#include "structs.h"
#include "enums.h"
#include "util.h"

inline uintptr_t CurlEasyAdd;
inline uintptr_t CurlSetAdd;
inline uintptr_t ProcessEventAdd;
inline uintptr_t GEngineAdd;
inline uintptr_t SCOIAdd;
inline uintptr_t GONIAdd;

void* (*ProcessEvent)(void*, void*, void*, void*) = nullptr;
FString(*GetObjectNameInternal)(PVOID) = nullptr;

namespace Hooks
{
	bool init()
	{
#ifdef SSL_BYPASS

		CurlEasyAdd = Util::FindPattern(Patterns::CurlEasySetOpt, Masks::CurlEasySetOpt);
		VALIDATE_ADDRESS(CurlEasyAdd, "Failed to find Curl Easy Address.");

		CurlSetAdd = Util::FindPattern(Patterns::CurlSetOpt, Masks::CurlSetOpt);
		VALIDATE_ADDRESS(CurlSetAdd, "Failed to find Curl SetOpt Address.");

#endif

#ifdef PROCESS_EVENT

		ProcessEventAdd = Util::FindPattern(Patterns::ProcessEvent, Masks::ProcessEvent);
		VALIDATE_ADDRESS(ProcessEventAdd, "Failed to find ProcessEvent Address.");

		GONIAdd = Util::FindPattern(Patterns::GONI, Masks::GONI);
		VALIDATE_ADDRESS(GONIAdd, "Failed to find GetObjectName Address.");

		GetObjectNameInternal = decltype(GetObjectNameInternal)(GONIAdd);

#endif

#ifdef UNLOCK_CONSOLE

		GEngineAdd = Util::FindPattern(Patterns::GEngine, Masks::GEngine);
		VALIDATE_ADDRESS(GEngineAdd, "Failed to find GEngine Address.");

		SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, "Failed to find SCOI Address.");

#endif
		return true;
	}
}

#ifdef PROCESS_EVENT

std::wstring GetObjectName(UObject* object) {
	
	auto internalName = GetObjectNameInternal(object);
	if (!internalName.c_str()) return L"";

	std::wstring name(internalName.c_str());

	return name;
}

void* ProcessEventDetour(UObject* pObj, UObject* pFunc, void* pParams, void* pRes)
{
	if (pObj && pFunc)
	{
		auto nObj = GetObjectName(pObj);
	    auto nFunc = GetObjectName(pFunc);
		if (!wcsstr(nFunc.c_str(), L"Tick"))
		{
			printf("\nObject: %ws\nFunction: %ws\n", nObj.c_str(), nFunc.c_str());

		}
	}

	return ProcessEvent(pObj, pFunc, pParams, pRes);
}

#endif
