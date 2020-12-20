#pragma once

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
