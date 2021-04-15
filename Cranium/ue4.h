/**
 * Copyright (c) 2020-2021 Kareem Olim (Kemo)
 * All Rights Reserved. Licensed under the Neo License
 * https://neonite.dev/LICENSE.html
 */

#pragma once

inline void* (*ProcessEvent)(void*, void*, void*);
inline UObject* (*SpawnActor)(UObject* UWorld, UClass* Class, FTransform const* UserTransformPtr, const FActorSpawnParameters& SpawnParameters);
inline void (*GetFullName)(FField* Obj, FString& ResultString, const UObject* StopOuter, EObjectFullNameFlags Flags);
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

inline UObject* (*StaticLoadObject)(
	UClass* ObjectClass,
	UObject* InOuter,
	const TCHAR* InName,
	const TCHAR* Filename,
	uint32_t LoadFlags,
	void* Sandbox,
	bool bAllowObjectReconciliation,
	void* InstancingContext
);

inline UObject* KismetRenderingLibrary;
inline UObject* KismetStringLibrary;


namespace UE4
{
	inline auto StaticLoadObjectEasy(UClass* inClass, const wchar_t* inName, UObject* inOuter = nullptr)
	{
		return StaticLoadObject(inClass, inOuter, inName, nullptr, 0, nullptr, false, nullptr);
	}

	//Frees the memory for the name
	inline void Free(void* buffer)
	{
		//FreeInternal(buffer);
		//https://en.cppreference.com/w/cpp/memory/c/free
		std::free(buffer); //assuming cstdlib is included 
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
	static T FindObject(wchar_t const* name, bool ends_with = false, bool to_lower = false, int toSkip = 0)
	{
		for (auto i = 0x0; i < GObjs->NumElements; ++i)
		{
			auto object = GObjs->GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}

			std::wstring objectFullName = object->GetFullName();

			if (to_lower)
			{
				std::transform(objectFullName.begin(), objectFullName.end(), objectFullName.begin(),
				               [](const unsigned char c) { return std::tolower(c); });
			}

			if (!ends_with)
			{
				if (objectFullName.starts_with(name))
				{
					if (toSkip > 0)
					{
						toSkip--;
					}
					else
					{
						return reinterpret_cast<T>(object);
					}
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
			auto object = GObjs->GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}
			std::wstring className = object->Class->GetFullName();
			std::wstring objectName = object->GetFullName();
			std::wstring item = L"\n[" + std::to_wstring(i) + L"] Object:[" + objectName + L"] Class:[" + className + L"]\n";
			log << item;
		}
	}

	inline void DumpBPs()
	{
		std::wofstream log("Blueprints.log");
		for (auto i = 0x0; i < GObjs->NumElements; ++i)
		{
			auto object = GObjs->GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}

			auto ClassName = object->Class->GetName();

			if (ClassName == XOR(L"BlueprintGeneratedClass"))
			{
				auto objectNameW = object->GetName();
				log << objectNameW + L"\n";
			}
		}
		log.flush();
	}
}
