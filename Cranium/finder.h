/**
 * Copyright (c) 2020-2021 Kareem Olim (Kemo)
 * All Rights Reserved. Licensed under the Neo License
 * https://neonite.dev/LICENSE.html
 */

#pragma once
#include "pch.h"
#include "structs.h"

namespace Offsets
{
	inline static constexpr int32_t OffsetInternal = 0x4C;
	inline static constexpr int32_t Next = 0x20;
	inline static constexpr int32_t ClassPrivate = 0x10;
	inline static constexpr int32_t ChildProperties = 0x50;
	inline static constexpr int32_t SuperStruct = 0x40;
}

class GameObject;

class GameClass
{
public:
	GameObject* GetChildProperties()
	{
		return *reinterpret_cast<GameObject**>(this + Offsets::ChildProperties);
	}

	GameClass* GetSuperStruct()
	{
		return *reinterpret_cast<GameClass**>(this + Offsets::SuperStruct);
	}
};

class GameObject
{
public:
	GameClass* GetClass()
	{
		return *reinterpret_cast<GameClass**>(this + Offsets::ClassPrivate);
	}

	GameObject* GetNext()
	{
		return *reinterpret_cast<GameObject**>(this + Offsets::Next);
	}

	int32_t GetOffsetInternal()
	{
		return *reinterpret_cast<int32_t*>(this + Offsets::OffsetInternal);
	}
};

class ObjectFinder
{
	std::wstring m_currentObject;
	std::wstring m_objectType;
	GameObject* m_object;
	GameObject*& m_objectRef;

	static GameObject* InternalFindChildInObject(GameObject* inObject, std::wstring_view childName)
	{
		GameObject* propertyObject = nullptr;
		GameObject* next = inObject->GetNext();
		if (next == nullptr) return nullptr;

		auto firstPropertyName = UE4::GetFirstName(reinterpret_cast<FField*>(inObject));

		//printf("\n firstPropertyName: %ls \n", firstPropertyName.c_str());

		if (firstPropertyName == childName)
		{
			return inObject;
		}

		while (next)
		{
			std::wstring nextName = UE4::GetFirstName(reinterpret_cast<FField*>(next));

			//printf("\n nextName: %ls \n", nextName.c_str());

			if (childName == nextName)
			{
				propertyObject = next;
				break;
			}
			else
			{
				next = next->GetNext();
			}
		}

		return propertyObject;
	}

	GameObject*& resolveValuePointer(GameObject* bastePtr, GameObject* prop) const
	{
		//printf("\nObject: %ls, Offset: %x\n", GetObjectFirstName(reinterpret_cast<UObject*>(m_object)).c_str(), prop->GetOffsetInternal());
		return *reinterpret_cast<GameObject**>(reinterpret_cast<uintptr_t>(m_object) + prop->GetOffsetInternal());
	}

	GameObject*& resolveArrayValuePointer(GameObject* bastePtr, GameObject* prop) const
	{
		return *reinterpret_cast<GameObject**>(*reinterpret_cast<GameObject**>(reinterpret_cast<uintptr_t>(m_object) + prop->GetOffsetInternal()));
	}

public:
	ObjectFinder(const std::wstring& currentObject, const std::wstring objectType, GameObject* object, GameObject*& objectRef) :
		m_currentObject(currentObject), m_objectType(objectType), m_object(object), m_objectRef(objectRef)
	{
	};

	UObject*& GetObj() const
	{
		return reinterpret_cast<UObject*&>(m_objectRef);
	}

	static ObjectFinder EntryPoint(uintptr_t EntryPointAddress)
	{
		return ObjectFinder{L"EntryPoint", L"None", reinterpret_cast<GameObject*>(EntryPointAddress), reinterpret_cast<GameObject*&>(EntryPointAddress)};
	}

	ObjectFinder Find(const std::wstring& objectToFind) const
	{
		return FindChildObject(objectToFind);
	}

	static auto FindOffset(const std::wstring& classToFind, const std::wstring& objectToFind)
	{
		auto Class = UE4::FindObject<UClass*>(classToFind.c_str(), true);

		if (Class)
		{
			GameObject* property = InternalFindChildInObject(reinterpret_cast<GameObject*>(Class->ChildProperties), objectToFind);
			if (property)
			{
				//printf("[ObjectFinder] Found %ls at 0x%x", objectToFind.c_str(), property->GetOffsetInternal());
				return property->GetOffsetInternal();
			}
		}

		return 0;
	}

	ObjectFinder FindChildObject(const std::wstring& objectToFind) const
	{
		GameClass* classPrivate = m_object->GetClass();
		GameObject* childProperties = classPrivate->GetChildProperties();
		GameObject* propertyFound = nullptr;

		if (childProperties)
		{
			propertyFound = InternalFindChildInObject(childProperties, objectToFind);
		}

		GameClass* superStruct = classPrivate->GetSuperStruct();

		while (superStruct && !propertyFound)
		{
			childProperties = superStruct->GetChildProperties();
			if (childProperties)
			{
				propertyFound = InternalFindChildInObject(childProperties, objectToFind);
				if (propertyFound) break;
			}
			superStruct = superStruct->GetSuperStruct();
		}

		GameObject* valuePtr = resolveValuePointer(m_object, propertyFound);

		const std::wstring type = UE4::GetFieldClassName(reinterpret_cast<FField*>(propertyFound));

		if (type == XOR(L"ArrayProperty"))
		{
			//this will return the first element in the array
			//TODO: recode this part
			valuePtr = *reinterpret_cast<GameObject**>(valuePtr);

			GameObject*& valuePtrRef = resolveArrayValuePointer(m_object, propertyFound);
			return ObjectFinder(objectToFind, type, valuePtr, valuePtrRef);
		}
		else
		{
			GameObject*& valuePtrRef = resolveValuePointer(m_object, propertyFound);
			return ObjectFinder(objectToFind, type, valuePtr, valuePtrRef);
		}
	}

	static UObject* FindActor(std::wstring name, int toSkip = 0)
	{
		ObjectFinder EngineFinder = EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));
		ObjectFinder PersistentLevelFinder = WorldFinder.Find(XOR(L"PersistentLevel"));

		const DWORD AActors = 0x98;

		for (auto i = 0x00; i < READ_DWORD(PersistentLevelFinder.GetObj(), AActors + sizeof(void*)); i++)
		{
			auto Actors = READ_POINTER(PersistentLevelFinder.GetObj(), AActors);

			auto pActor = static_cast<UObject*>(READ_POINTER(Actors, i * sizeof(void*)));

			if (pActor != nullptr)
			{
				//printf("\n[Actor %i] %ls\n", i, pActor->GetFullName().c_str());

				if (pActor->GetFullName().starts_with(name))
				{
					if (toSkip > 0)
					{
						toSkip--;
					}
					else
					{
						printf("\n[NeoRoyale] %ls was found!.\n", name.c_str());
						return pActor;
					}
				}
			}
		}

		return nullptr;
	}
};


namespace UE4
{
	inline auto SpawnActorEasy(UClass* Class, FVector Location = FVector(), FQuat Rotation = FQuat())
	{
		FTransform Transform;
		Transform.Scale3D = FVector(1, 1, 1);
		Transform.Translation = Location;
		Transform.Rotation = Rotation;

		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));

		return SpawnActor(
			WorldFinder.GetObj(),
			Class,
			&Transform,
			FActorSpawnParameters()
		);
	}
}
