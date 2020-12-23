#pragma once
#include "enums.h"

struct UClass
{
	BYTE unknown1[0x40];
	UClass* SuperClass;
};

struct UObject
{
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE unknown1[0x8];
	UObject* Outer;
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
