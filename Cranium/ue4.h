#pragma once

inline void* (*ProcessEvent)(void*, void*, void*);
inline int (*GetViewPoint)(void*, FMinimalViewInfo*, BYTE);
inline FString (*GetObjectNameInternal)(PVOID);
inline void (*GetFullName)(FField* Obj, FString& ResultString, const UObject* StopOuter, EObjectFullNameFlags Flags);
inline void (*GetObjectFullNameInternal)(UObject* Obj, FString& ResultString, const UObject* StopOuter, EObjectFullNameFlags Flags);
inline void (*FreeInternal)(void*);
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

//Frees the memory for the name
inline void Free(void* buffer)
{
	FreeInternal(buffer);
}

//Returns the very first name of the object (E.G: BP_PlayButton).
inline std::wstring GetObjectFirstName(UObject* object)
{
	const FString internalName = GetObjectNameInternal(object);
	if (!internalName.ToWString()) return L"";

	std::wstring name(internalName.ToWString());

	Free((void*)internalName.ToWString());

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

	Free((void*)s.ToWString());

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

		Free((void*)internalName.ToWString());
	}

	return name;
}

//Return FULL Object name including it's type.
inline std::wstring GetObjectFullName(UObject* object)
{
	FString s;
	GetObjectFullNameInternal(object, s, nullptr, EObjectFullNameFlags::None);
	std::wstring objectNameW = s.ToWString();

	Free((void*)s.ToWString());

	return objectNameW;
}

//Returns FField's type.
inline std::wstring GetFieldClassName(FField* obj)
{
	FString s;
	GetFullName(obj, s, nullptr, EObjectFullNameFlags::None);
	const std::wstring objectName = s.ToWString();
	auto className = Util::sSplit(objectName, L" ");

	Free((void*)s.ToWString());

	return className;
}

//Find any entity inside the UGlobalObjects array aka. GObjects.
template <typename T>
static T FindObject(wchar_t const* name, bool ends_with = false, bool to_lower = false)
{
	for (auto i = 0x0; i < GObjs->NumElements; ++i)
	{
		const auto object = GObjs->GetByIndex(i);
		if (object == nullptr)
		{
			continue;
		}

		auto objectFullName = GetObjectFullName(object);

		if (to_lower)
		{
			std::transform(objectFullName.begin(), objectFullName.end(), objectFullName.begin(),
			               [](const unsigned char c) { return std::tolower(c); });
		}

		if (!ends_with)
		{
			if (objectFullName.starts_with(name))
			{
				return reinterpret_cast<T>(object);
			}
		}
		else
		{
			if (objectFullName.ends_with(name))
			{
				return reinterpret_cast<T>(object);
			}
		}
	}
	return nullptr;
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
