#pragma once
#include "pch.h"
#include "structs.h"
#include "enums.h"
#include "util.h"

inline uintptr_t CurlEasyAdd;
inline uintptr_t CurlSetAdd;
inline uintptr_t ProcessEventAdd;
inline uintptr_t GEngineAdd;
inline uintptr_t GObjectsAdd;
inline uintptr_t SCOIAdd;
inline uintptr_t GONIAdd;

void* (*ProcessEvent)(void*, void*, void*) = nullptr;
FString (*GetObjectNameInternal)(PVOID) = nullptr;
GObjects* GObjs = nullptr;


namespace Hooks
{
#ifdef SSL_BYPASS

	bool curl()
	{
		CurlEasyAdd = Util::FindPattern(Patterns::CurlEasySetOpt, Masks::CurlEasySetOpt);
		VALIDATE_ADDRESS(CurlEasyAdd, "Failed to find Curl Easy Address.");

		CurlSetAdd = Util::FindPattern(Patterns::CurlSetOpt, Masks::CurlSetOpt);
		VALIDATE_ADDRESS(CurlSetAdd, "Failed to find Curl SetOpt Address.");
	}

#endif

	bool init()
	{
		if (MH_Initialize() != MH_OK)
		{
			MessageBoxA(0, "Failed to initialize min-hook, terminating the thread.", "Cranium", MB_OK);
			FreeLibraryAndExitThread(GetModuleHandle(NULL), 0);
		}

		ProcessEventAdd = Util::FindPattern(Patterns::ProcessEvent, Masks::ProcessEvent);
		VALIDATE_ADDRESS(ProcessEventAdd, "Failed to find ProcessEvent Address.");

		GONIAdd = Util::FindPattern(Patterns::GONI, Masks::GONI);
		VALIDATE_ADDRESS(GONIAdd, "Failed to find GetObjectName Address.");

		GetObjectNameInternal = decltype(GetObjectNameInternal)(GONIAdd);

		GEngineAdd = Util::FindPattern(Patterns::GEngine, Masks::GEngine);
		VALIDATE_ADDRESS(GEngineAdd, "Failed to find GEngine Address.");

		GObjectsAdd = Util::FindPattern(Patterns::GObjects, Masks::GObjects);
		VALIDATE_ADDRESS(GObjectsAdd, "Failed to find GObjects Address.");

		GObjs = decltype(GObjs)(RELATIVE_ADDRESS(GObjectsAdd, 7));

		SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, "Failed to find SCOI Address.");

		return true;
	}
}


std::wstring GetObjectFirstName(UObject* object)
{
	FString internalName = GetObjectNameInternal(object);
	if (!internalName.c_str()) return L"";

	std::wstring name(internalName.c_str());

	return name;
}

std::wstring GetObjectName(UObject* object)
{
	std::wstring name(L"");
	for (auto i = 0; object; object = object->Outer, ++i)
	{
		FString internalName = GetObjectNameInternal(object);
		if (!internalName.c_str()) break;

		name = internalName.c_str() + std::wstring(i > 0 ? L"." : L"") + name;
	}

	return name;
}

void* FindObject(wchar_t const* name)
{
	for (auto array : GObjs->ObjectArray->Objects)
	{
		auto fuObject = array;
		for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject)
		{
			auto object = fuObject->Object;
			if (object->ObjectFlags != 0x41)
			{
				continue;
			}

			if (GetObjectName(object) == name)
			{
				return object;
			}
		}
	}

	return nullptr;
}
