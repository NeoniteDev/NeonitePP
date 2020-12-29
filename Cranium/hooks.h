#pragma once
#include "pch.h"
#include "structs.h"
#include "enums.h"
#include "util.h"


inline uintptr_t CurlEasyAdd;
inline uintptr_t CurlSetAdd;
inline uintptr_t ProcessEventAdd;
inline uintptr_t GetViewPointAdd;
inline uintptr_t GEngineAdd;
inline uintptr_t GObjectsAdd;
inline uintptr_t GNamesAdd;
inline uintptr_t FNameToStringAdd;
inline uintptr_t UWorldAdd;
inline uintptr_t SCOIAdd;
inline uintptr_t GONIAdd;

void* (*ProcessEvent)(void*, void*, void*) = nullptr;
int (*GetViewPoint)(void*, FMinimalViewInfo*, BYTE) = nullptr;
FString (*GetObjectNameInternal)(PVOID) = nullptr;
GObjects* GObjs = nullptr;
void (*FNameToString)(FName* pThis, FString& out);
UEngine* GEngine;
void** UWorld;

typedef UObject* (__fastcall* f_StaticConstructObject_Internal)(
	UClass* Class,
	UObject* InOuter,
	void* Name,
	EObjectFlags SetFlags,
	EInternalObjectFlags InternalSetFlags,
	UObject* Template,
	bool bCopyTransientsFromClassDefaults,
	void* InstanceGraph,
	bool bAssumeTemplateIsArchetype
);

static f_StaticConstructObject_Internal StaticConstructObject_Internal;


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
			FreeLibraryAndExitThread(GetModuleHandle(nullptr), 0);
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

		FNameToStringAdd = Util::FindPattern(Patterns::FNameToString, Masks::FNameToString);
		VALIDATE_ADDRESS(FNameToStringAdd, "Failed to find FNameToString Address.");

		FNameToString = decltype(FNameToString)(FNameToStringAdd);

		UWorldAdd = Util::FindPattern(Patterns::UWorld, Masks::UWorld);
		VALIDATE_ADDRESS(UWorldAdd, "Failed to find UWorld Address.");

		UWorld = decltype(UWorld)(RELATIVE_ADDRESS(UWorldAdd, 7));

		SCOIAdd = Util::FindPattern(Patterns::SCOI, Masks::SCOI);
		VALIDATE_ADDRESS(SCOIAdd, "Failed to find SCOI Address.");


		GetViewPointAdd = Util::FindPattern(Patterns::GetViewPoint, Masks::GetViewPoint);
		VALIDATE_ADDRESS(GetViewPointAdd, "Failed to find GetViewPoint Address.");

		return true;
	}
}


std::wstring GetObjectFirstName(UObject* object)
{
	FString internalName = GetObjectNameInternal(object);
	if (!internalName.ToString()) return L"";

	std::wstring name(internalName.ToString());

	return name;
}

std::wstring GetObjectName(UObject* object)
{
	std::wstring name(L"");
	for (auto i = 0; object; object = object->Outer, ++i)
	{
		FString internalName = GetObjectNameInternal(object);
		if (!internalName.ToString()) break;
		name = internalName.ToString() + std::wstring(i > 0 ? L"." : L"") + name;
	}

	return name;
}

void DumpAllGObjects()
{
	for (auto array : GObjs->ObjectArray->Objects)
	{
		if (array == nullptr)
		{
			continue;
		}
		auto fuObject = array;
		for (auto i = 0x0; i < GObjs->ObjectCount && fuObject->Object; ++i, ++fuObject)
		{
			auto object = fuObject->Object;
			if (object->ObjectFlags != 0x41)
			{
				auto className = GetObjectName((UObject*)object->Class).c_str();
				auto objectName = GetObjectName(object).c_str();
				printf("\n[%i] Object:[%ws] Class:[%ws]\n", i, objectName, className);
			}
		}
	}
}

template <typename T>
static T FindObject(wchar_t const* name)
{
	for (auto array : GObjs->ObjectArray->Objects)
	{
		if (array == nullptr)
		{
			continue;
		}

		auto fuObject = array;
		for (auto i = 0x0; i < GObjs->ObjectCount && fuObject->Object; ++i, ++fuObject)
		{
			auto object = fuObject->Object;
			if (object->ObjectFlags == 0x41)
			{
				break;
			}

			if (GetObjectName(object) == name)
			{
				return reinterpret_cast<T>(object);
			}
		}
	}
	return nullptr;
}

void DumpUnversioned()
{
	auto ACID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaCharacterItemDefinition");
	auto props = ACID->SuperStruct->ChildProperties;
	FString s;
	FNameToString(&props->NamePrivate, s);
	printf("\n\n\n\n\n\n\nPrivateName: %ls\n\n\n\n\n\n\n", s.ToString());
}

void DumpIDs()
{
	UClass* CID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaCharacterItemDefinition");
	UClass* BID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaBackpackItemDefinition");
	UClass* PCID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaPetCarrierItemDefinition");
	UClass* EID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaEmojiItemDefinition");
	UClass* DID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaDanceItemDefinition");
	UClass* PID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaPickaxeItemDefinition");
	UClass* GID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaGliderItemDefinition");
	UClass* SDID = FindObject<UClass*>(L"/Script/FortiteGame.AthenaSkyDiveContrailItemDefinition");
	UClass* TID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaToyItemDefinition");
	UClass* IWD = FindObject<UClass*>(L"/Script/FortniteGame.AthenaItemWrapDefinition");
	UClass* LSID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaLoadingScreenItemDefinition");
	UClass* MPID = FindObject<UClass*>(L"/Script/FortniteGame.AthenaMusicPackItemDefinition");
	for (auto array : GObjs->ObjectArray->Objects)
	{
		if (array == nullptr) continue;
		auto fuObject = array;
		for (DWORD i = 0x0; i < GObjs->ObjectCount && fuObject->Object; i++, fuObject++)
		{
			auto object = fuObject->Object;
			if (object->IsA(CID) ||
				object->IsA(BID) ||
				object->IsA(PCID) ||
				object->IsA(EID) ||
				object->IsA(DID) ||
				object->IsA(PID) ||
				object->IsA(GID) ||
				object->IsA(SDID) ||
				object->IsA(TID) ||
				object->IsA(IWD) ||
				object->IsA(LSID) ||
				object->IsA(MPID))
			{
				auto objectNameW = GetObjectName(object);
				std::string objectName = std::string(objectNameW.begin(), objectNameW.end());
				std::string id = objectName.substr(objectName.find_last_of(".") + 1);
				if (id.starts_with("Default__")) break;
				printf("\n%s\n", id.c_str());
			}
		}
	}
}
