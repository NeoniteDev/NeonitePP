#pragma once
#include "enums.h"
#include <set>

template <class T>
struct TArray
{
	friend struct FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	int Num() const
	{
		return Count;
	};

	T& operator[](int i)
	{
		return Data[i];
	};

	const T& operator[](int i) const
	{
		return Data[i];
	};

	bool IsValidIndex(int i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : private TArray<wchar_t>
{
	FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	}

	bool IsValid() const
	{
		return Data != nullptr;
	}

	const wchar_t* ToWString() const
	{
		return Data;
	}

	std::string ToString() const
	{
		const auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

template <class TEnum>
class TEnumAsByte
{
public:
	TEnumAsByte()
	{
	}

	TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	operator TEnum() const
	{
		return static_cast<TEnum>(value);
	}

	TEnum GetValue() const
	{
		return static_cast<TEnum>(value);
	}

private:
	uint8_t value;
};

struct FName
{
	uint32_t ComparisonIndex;
	uint32_t DisplayIndex;

	FName() = default;

	explicit FName(int64_t name)
	{
		DisplayIndex = (name & 0xFFFFFFFF00000000LL) >> 32;
		ComparisonIndex = (name & 0xFFFFFFFFLL);
	};

	void ToString(FString& Out);
};


struct UClass;

struct UObject
{
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	FName NamePrivate;
	UObject* Outer;

	bool IsA(UClass* cmp) const
	{
		if (this->Class == cmp)
		{
			return true;
		}
		return false;
	}

	FName GetFName() const
	{
		return *reinterpret_cast<const FName*>(this + 0x18);
	}
};

struct FField
{
	void* vtable;
	void* padding_01;
	void* ClassPrivate;
	void* Owner;
	FField* Next;
	FName NamePrivate;
	EObjectFlags FlagsPrivate;
};

struct FProperty : FField
{
	int32_t ArrayDim;
	int32_t ElementSize;
	EPropertyFlags PropertyFlags;
	uint16_t RepIndex;
	TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;
	int32_t Offset_Internal;
	FName RepNotifyFunc;
	FProperty* PropertyLinkNext;
	FProperty* NextRef;
	FProperty* DestructorLinkNext;
	FProperty* PostConstructLinkNext;
};

struct UField : UObject
{
	UField* Next;
	void* padding_01;
	void* padding_02;
};

struct UStruct : UField
{
	UStruct* SuperStruct;
	UField* Children;
	FField* ChildProperties;
	int32_t PropertiesSize;
	int32_t MinAlignment;
	TArray<uint8_t> Script;
	FProperty* PropertyLink;
	FProperty* RefLink;
	FProperty* DestructorLink;
	FProperty* PostConstructLink;
};

struct UClass : UStruct
{
};

struct UFunction : UStruct
{
	int32_t FunctionFlags;
	int16_t RepOffset;
	int8_t NumParms;
	char unknown1[1];
	int16_t ParmsSize;
	int16_t ReturnValueOffset;
	int16_t RPCId;
	int16_t RPCResponseId;
	class UProperty* FirstPropertyToInit;
	UFunction* EventGraphFunction;
	int32_t EventGraphCallOffset;
	void* Func;
};


struct UCheatManager_Summon_Params
{
	FString ClassName;
};

struct UCheatManager_DestroyAll_Params
{
	UClass* Class;
};

struct APlayerController_SwitchLevel_Params
{
	FString URL;
};

struct AController_Possess_Params
{
	UObject* InPawn;
};

struct ACharacter_IsJumpProvidingForce_Params
{
	bool ReturnValue;
};

struct Empty_Params
{
};

struct UCheatManager_CheatScript_Params
{
	FString ScriptName;
};

struct AFortPlayerPawn_BeginSkydiving_Params
{
	bool bFromBus;
};

struct UCheatManager_BugItGo_Params
{
	float X;
	float Y;
	float Z;
	float Pitch;
	float Yaw;
	float Roll;
};


struct FUObjectItem
{
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

struct PreFUObjectItem
{
	FUObjectItem* FUObject[10];
};

struct GObjects
{
	PreFUObjectItem* ObjectArray;
	BYTE unknown1[8];
	int32_t MaxElements;
	int32_t NumElements;

	void NumChunks(int* start, int* end) const
	{
		int cStart = 0, cEnd = 0;

		if (!cEnd)
		{
			while (true)
			{
				if (ObjectArray->FUObject[cStart] == nullptr)
				{
					cStart++;
				}
				else
				{
					break;
				}
			}

			cEnd = cStart;
			while (true)
			{
				if (ObjectArray->FUObject[cEnd] == nullptr)
				{
					break;
				}
				cEnd++;
			}
		}

		*start = cStart;
		*end = cEnd;
	}

	UObject* GetByIndex(int32_t index) const
	{
		int cStart = 0, cEnd = 0;
		int chunkIndex, chunkSize = 0xFFFF, chunkPos;
		FUObjectItem* Object;

		NumChunks(&cStart, &cEnd);

		chunkIndex = index / chunkSize;
		if (chunkSize * chunkIndex != 0 &&
			chunkSize * chunkIndex == index)
		{
			chunkIndex--;
		}

		chunkPos = cStart + chunkIndex;
		if (chunkPos < cEnd)
		{
			Object = ObjectArray->FUObject[chunkPos] + (index - chunkSize * chunkIndex);
			if (!Object) { return nullptr; }

			return Object->Object;
		}

		return nullptr;
	}
};

struct UConsole;

struct UCheatManager
{
	class ADebugCameraController* DebugCameraControllerRef;
	class UClass* DebugCameraControllerClass;
	unsigned char UnknownData00[0x40];
};

struct UGameViewportClient
{
	CHAR unknown1[0x40];
	UConsole* ViewportConsole;
	TArray<struct FDebugDisplayProperty> DebugProperties;
	unsigned char UnknownData01[0x10];
	int MaxSplitscreenPlayers;
	unsigned char UnknownData02[0xC];
	class UWorld* World;
	class UGameInstance* GameInstance;
	unsigned char UnknownData03[0x2B8];
};

struct UEngine
{
	CHAR unknown1[0xF8];
	UClass* ConsoleClass;
	CHAR unknown2[0x688];
	UGameViewportClient* GameViewportClient;
};


struct FVector
{
	float X;
	float Y;
	float Z;

	FVector()
		: X(0), Y(0), Z(0)
	{
	}

	FVector(float x, float y, float z)
		: X(x),
		  Y(y),
		  Z(z)
	{
	}

	FVector operator-(FVector v)
	{
		return FVector(X - v.X, Y - v.Y, Z - v.Z);
	}

	FVector operator+(FVector v)
	{
		return FVector(X + v.X, Y + v.Y, Z + v.Z);
	}

	float Distance(FVector v)
	{
		return ((X - v.X) * (X - v.X) +
			(Y - v.Y) * (Y - v.Y) +
			(Z - v.Z) * (Z - v.Z));
	}
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;
};

struct FQuat
{
	float W, X, Y, Z;
};

template <class OptionalType>
struct TOptional
{
};

struct FTransform
{
	FQuat Rotation;
	FVector Scale3D;
	FVector Translation;
};

struct FMinimalViewInfo
{
	struct FVector Location;
	struct FRotator Rotation;
	float FOV;
	float DesiredFOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float OrthoFarClipPlane;
	float AspectRatio;
	unsigned char bConstrainAspectRatio : 1;
	unsigned char bUseFieldOfViewForLOD : 1;
	unsigned char PreviousViewTransform[0x3];
	TEnumAsByte<ECameraProjectionMode> ProjectionMode;
};

struct FWeakObjectPtr
{
public:
	bool SerialNumbersMatch(FUObjectItem* ObjectItem) const
	{
		return ObjectItem->SerialNumber == ObjectSerialNumber;
	}

	bool IsValid() const;

	UObject* Get() const;

	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

struct ULevelStreamingDynamic_LoadLevelInstance_Params
{
	UObject* WorldContextObject;
	FString LevelName;
	FVector Location;
	FRotator Rotation;
	bool bOutSuccess;
	FString OptionalLevelNameOverride;
};

template <class T, class TWeakObjectPtrBase = FWeakObjectPtr>
struct TWeakObjectPtr : private TWeakObjectPtrBase
{
public:
	T* Get() const
	{
		return static_cast<T*>(TWeakObjectPtrBase::Get());
	}

	T& operator*() const
	{
		return *Get();
	}

	T* operator->() const
	{
		return Get();
	}

	bool IsValid() const
	{
		return TWeakObjectPtrBase::IsValid();
	}
};
