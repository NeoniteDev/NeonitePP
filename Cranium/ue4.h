#pragma once

inline void* (*ProcessEvent)(void*, void*, void*);
inline int (*GetViewPoint)(void*, FMinimalViewInfo*, BYTE);
inline FString (*GetObjectNameInternal)(PVOID);
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
	const FString internalName = GetObjectNameInternal(object);
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
inline std::wstring GetFieldClassName(FField* obj)
{
	FString s;
	GetFullName(obj, s, nullptr, EObjectFullNameFlags::None);
	const std::wstring objectName = s.ToWString();
	auto className = Util::sSplit(objectName, L" ");
	return className;
}

//Find any entity inside the UGlobalObjects array aka. GObjects.
template <typename T>
static T FindObject(wchar_t const* name)
{
	printf("\nfuck me: %i\n", GObjs->NumElements);
	for (auto i = 0x0; i < GObjs->NumElements; ++i)
	{
		const auto object = GObjs->GetByIndex(i);
		if (object == nullptr)
		{
			continue;
		}

		if (GetObjectFullName(object).starts_with(name))
		{
			printf("\nNIGGER %ls\n", GetObjectFullName(object).c_str());
			return reinterpret_cast<T>(object);
		}
		else
		{
			printf("fuck");
		}
	}
	return nullptr;
}

inline bool DumpIDs()
{
	std::wofstream log(XOR(L"ids.config"), std::ios::trunc);

	//TODO: Better way.
	//THIS FUCKING IS SO BAD CODED BUT IDK WHAT ELSE TO DO
	UClass* CID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaCharacterItemDefinition"));
	UClass* BID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaBackpackItemDefinition"));
	UClass* PCID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaPetCarrierItemDefinition"));
	UClass* EID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaEmojiItemDefinition"));
	UClass* DID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaDanceItemDefinition"));
	UClass* PID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaPickaxeItemDefinition"));
	UClass* GID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaGliderItemDefinition"));
	UClass* SDID = FindObject<UClass*>(XOR(L"Class /Script/FortiteGame.AthenaSkyDiveContrailItemDefinition"));
	UClass* TID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaToyItemDefinition"));
	UClass* IWD = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaItemWrapDefinition"));
	UClass* LSID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaLoadingScreenItemDefinition"));
	UClass* MPID = FindObject<UClass*>(XOR(L"Class /Script/FortniteGame.AthenaMusicPackItemDefinition"));
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
				auto id = GetObjectFirstName(object);
				auto def = GetObjectFirstName(object->Class);

				std::wstring r = L"ItemDefinition";

				//handle wraps
				if (object->IsA(IWD)) r = L"Definition";

				std::wstring::size_type i = def.find(r);
				if (i != std::string::npos)
				{
					def.erase(i, r.length());
				}
				if (id.starts_with(XOR(L"Default__"))) break;
				log << def << ":" << id << L"\n";
			}
		}
	}
	log.flush(); //make sure it outputted everything.
	return true;
}

inline void DumpGObjects()
{
	std::wofstream log("GObjects.log");

	for (auto i = 0x0; i < GObjs->NumElements; ++i)
	{
		const auto object = GObjs->GetByIndex(i);
		if (object == nullptr)
		{
			continue;
		}
		std::wstring className = GetObjectName(static_cast<UObject*>(object->Class)).c_str();
		std::wstring objectName = GetObjectFullName(object).c_str();
		std::wstring item = L"\n[" + std::to_wstring(i) + L"] Object:[" + objectName + L"] Class:[" + className + L"]\n";
		log << item;
	}
	log.flush();
}

inline void DumpBPs()
{
	std::wofstream log("Blueprints.log");
	for (auto i = 0x0; i < GObjs->NumElements; ++i)
	{
		const auto object = GObjs->GetByIndex(i);
		if (object == nullptr)
		{
			continue;
		}

		auto ClassName = GetObjectFirstName(object->Class);

		if (ClassName == XOR(L"BlueprintGeneratedClass"))
		{
			auto objectNameW = GetObjectFirstName(object);
			log << objectNameW + L"\n";
		}
	}
	log.flush();
}
