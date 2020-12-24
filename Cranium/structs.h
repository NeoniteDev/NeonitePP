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

	INT Num() const
	{
		return Count;
	};

	T& operator[](INT i)
	{
		return Data[i];
	};

	BOOLEAN IsValidIndex(INT i)
	{
		return i < Num();
	}

private:
	T* Data;
	INT Count;
	INT Max;
};

template <class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

class FNameEntry
{
public:
	static const auto NAME_WIDE_MASK = 0x1;
	static const auto NAME_INDEX_SHIFT = 1;

	int32_t Index;
	char UnknownData00[0x04];
	FNameEntry* HashNext;

	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};

	inline const int32_t GetIndex() const
	{
		return Index >> NAME_INDEX_SHIFT;
	}

	inline bool IsWide() const
	{
		return Index & NAME_WIDE_MASK;
	}

	inline const char* GetAnsiName() const
	{
		return AnsiName;
	}

	inline const wchar_t* GetWideName() const
	{
		return WideName;
	}
};

template <typename ElementType, int32_t MaxTotalElements, int32_t ElementsPerChunk>
class TStaticIndirectArrayThreadSafeRead
{
public:
	inline size_t Num() const
	{
		return NumElements;
	}

	inline bool IsValidIndex(int32_t index) const
	{
		return index < Num() && index >= 0;
	}

	inline ElementType const* const& operator[](int32_t index) const
	{
		return *GetItemPtr(index);
	}

private:
	inline ElementType const* const* GetItemPtr(int32_t Index) const
	{
		int32_t ChunkIndex = Index / ElementsPerChunk;
		int32_t WithinChunkIndex = Index % ElementsPerChunk;
		ElementType** Chunk = Chunks[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}

	enum
	{
		ChunkTableSize = (MaxTotalElements + ElementsPerChunk - 1) / ElementsPerChunk
	};

	ElementType** Chunks[ChunkTableSize];
	int32_t NumElements;
	int32_t NumChunks;
};

using TNameEntryArray = TStaticIndirectArrayThreadSafeRead<FNameEntry, 2 * 1024 * 1024, 16384>;

struct FName
{
	union
	{
		struct
		{
			int32_t ComparisonIndex;
			int32_t Number;
		};

		uint64_t CompositeComparisonValue;
	};

	inline FName()
		: ComparisonIndex(0),
		  Number(0)
	{
	};

	inline FName(int32_t i)
		: ComparisonIndex(i),
		  Number(0)
	{
	};

	FName(const char* nameToFind)
		: ComparisonIndex(0),
		  Number(0)
	{
		static std::set<int> cache;

		for (auto i : cache)
		{
			if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind))
			{
				ComparisonIndex = i;

				return;
			}
		}

		for (auto i = 0; i < GetGlobalNames().Num(); ++i)
		{
			if (GetGlobalNames()[i] != nullptr)
			{
				if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind))
				{
					cache.insert(i);

					ComparisonIndex = i;

					return;
				}
			}
		}
	};

	static TNameEntryArray* GNames;

	static inline TNameEntryArray& GetGlobalNames()
	{
		return *GNames;
	};

	inline const char* GetName() const
	{
		return GetGlobalNames()[ComparisonIndex]->GetAnsiName();
	};

	inline bool operator==(const FName& other) const
	{
		return ComparisonIndex == other.ComparisonIndex;
	};
};


struct FField
{
	BYTE ClassPrivate[0x8];
	BYTE Owner[0xC];
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

struct UStruct
{
	class UStruct* SuperStruct;
	class UField* Children;
	FField* ChildProperties;
	int32_t PropertiesSize;
	int32_t MinAlignment;
	TArray<uint8_t> Script;
	FProperty* PropertyLink;
	FProperty* RefLink;
	FProperty* DestructorLink;
	FProperty* PostConstructLink;
};

struct UClass
{
	BYTE unknown1[0x40];
	UStruct* SuperStruct;
};

struct UObject
{
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE unknown1[0x8];
	UObject* Outer;

	bool IsA(UClass* cmp) const
	{
		if (this->Class == cmp)
		{
			return true;
		}
		return false;
	}
};

struct FUObjectItem
{
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

struct TUObjectArray
{
	FUObjectItem* Objects[9];
};

struct GObjects
{
	TUObjectArray* ObjectArray;
	BYTE unknown1[0xC];
	DWORD ObjectCount;
};

struct UConsole;

struct UCheatManager
{
	CHAR unknown1[0x50];
};

struct UGameViewportClient
{
	CHAR unknown1[0x40];
	UConsole* ViewportConsole;
};

struct UEngine
{
	CHAR unknown1[0xF8];
	UClass* ConsoleClass;
	CHAR unknown2[0x688];
	UGameViewportClient* GameViewportClient;
};

struct FString : private TArray<WCHAR>
{
	FString()
	{
		Data = nullptr;
		Max = Count = 0;
	}

	FString(LPCWSTR other)
	{
		Max = Count = static_cast<INT>(wcslen(other));

		if (Count)
		{
			Data = const_cast<PWCHAR>(other);
		}
	};

	BOOLEAN IsValid()
	{
		return Data != nullptr;
	}

	PWCHAR c_str()
	{
		return Data;
	}
};


struct FVector
{
	float X;
	float Y;
	float Z;

	inline FVector()
		: X(0), Y(0), Z(0)
	{
	}

	inline FVector(float x, float y, float z)
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
