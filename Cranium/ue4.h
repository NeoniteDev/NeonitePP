#pragma once

inline void* (*ProcessEvent)(void*, void*, void*);
inline int (*GetViewPoint)(void*, FMinimalViewInfo*, BYTE);
inline FString(*GetObjectNameInternal)(PVOID);
inline void (*GetFullName)(FField* Obj, FString& ResultString, const UObject* StopOuter, EObjectFullNameFlags Flags);
inline void (*GetObjectFullNameInternal)(UObject* Obj, FString& ResultString, const UObject* StopOuter, EObjectFullNameFlags Flags);
inline GObjects* GObjs;
inline UEngine* GEngine;
inline UObject* (*StaticConstructObject)(
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

//Returns the very first name of the object (E.G: BP_PlayButton).
inline std::wstring GetObjectFirstName(UObject* object)
{
	FString internalName = GetObjectNameInternal(object);
	if (!internalName.ToWString()) return L"";

	std::wstring name(internalName.ToWString());

	return name;
}

//The same as above but for FFields.
inline std::wstring GetFirstName(FField* object)
{
	FString s;
	GetFullName(object, s, nullptr, EObjectFullNameFlags::None);
	std::wstring objectNameW = s.ToWString();

	std::wstring token;
	while (token != objectNameW)
	{
		token = objectNameW.substr(0, objectNameW.find_first_of(L":"));
		objectNameW = objectNameW.substr(objectNameW.find_first_of(L":") + 1);
	}

	return objectNameW;
}

//Returns Object Name (NOT FULL NAME, MORE LIKE A PATH).
inline std::wstring GetObjectName(UObject* object)
{
	std::wstring name(L"");
	for (auto i = 0; object; object = object->Outer, ++i)
	{
		FString internalName = GetObjectNameInternal(object);
		if (!internalName.ToWString()) break;
		name = internalName.ToWString() + std::wstring(i > 0 ? L"." : L"") + name;
	}

	return name;
}

//Return FULL Object name including it's type.
inline std::wstring GetObjectFullName(UObject* object)
{
	FString s;
	GetObjectFullNameInternal(object, s, nullptr, EObjectFullNameFlags::None);
	std::wstring objectNameW = s.ToWString();
	return objectNameW;
}

//Returns FField's type.
inline std::wstring GetFClassName(FField* obj)
{
	FString s;
	GetFullName(obj, s, nullptr, EObjectFullNameFlags::None);
	std::wstring objectNameW = s.ToWString();
	auto className = Util::sSplit(objectNameW, L" ");
	return className;
}

//Find any entity inside the UGlobalObjects array aka. GObjects.
template <typename T>
static T FindObject(wchar_t const* name)
{
	for (auto array : GObjs->ObjectArray->FUObject)
	{
		if (array == nullptr)
		{
			continue;
		}

		auto fuObject = array;
		for (auto i = 0x0; i < GObjs->NumElements && fuObject->Object; ++i, ++fuObject)
		{
			auto object = fuObject->Object;

			if (GetObjectFullName(object) == name)
			{
				return reinterpret_cast<T>(object);
			}
		}
	}
	return nullptr;
}


inline void DumpAllGObjects()
{
	std::ofstream log("GObjects.log");
	for (auto array : GObjs->ObjectArray->FUObject)
	{
		if (array == nullptr)
		{
			continue;
		}
		auto fuObject = array;
		for (auto i = 0x0; i < GObjs->NumElements && fuObject->Object; ++i, ++fuObject)
		{
			auto object = fuObject->Object;
			std::wstring classNameW = GetObjectName(static_cast<UObject*>(object->Class)).c_str();
			std::wstring objectNameW = GetObjectFullName(object).c_str();
			std::string objectName = std::string(objectNameW.begin(), objectNameW.end());
			std::string className = std::string(classNameW.begin(), classNameW.end());
			std::string item = "\n[" + std::to_string(i) + "] Object:[" + objectName + "] Class:[" + className + "]\n";
			log << item;
		}
	}
	log.flush();
}

inline void DumpIDs()
{
	std::ofstream log("ids.config", std::ios::trunc);

	//TODO: Better way.
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
	for (auto array : GObjs->ObjectArray->FUObject)
	{
		if (array == nullptr) continue;
		auto fuObject = array;
		for (DWORD i = 0x0; i < GObjs->NumElements && fuObject->Object; i++, fuObject++)
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
				log << id + "\n";
			}
		}
	}
	log.flush(); //make sure it outputted everything.
}
