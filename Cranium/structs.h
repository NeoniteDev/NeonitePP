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

struct FWeakObjectPtr
{
public:
	inline bool SerialNumbersMatch(FUObjectItem* ObjectItem) const
	{
		return ObjectItem->SerialNumber == ObjectSerialNumber;
	}

	bool IsValid() const;

	UObject* Get() const;

	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};


template<class T, class TWeakObjectPtrBase = FWeakObjectPtr>
struct TWeakObjectPtr : private TWeakObjectPtrBase
{
public:
	inline T* Get() const
	{
		return (T*)TWeakObjectPtrBase::Get();
	}

	inline T& operator*() const
	{
		return *Get();
	}

	inline T* operator->() const
	{
		return Get();
	}

	inline bool IsValid() const
	{
		return TWeakObjectPtrBase::IsValid();
	}
};


template<class TEnum>
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

struct FVector
{
	float                                              X;
	float                                              Y;
	float                                              Z;

	inline FVector()
		: X(0), Y(0), Z(0)
	{ }

	inline FVector(float x, float y, float z)
		: X(x),
		Y(y),
		Z(z)
	{ }

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
	float                                              Pitch;
	float                                              Yaw;
	float                                              Roll;
};

struct APlayerController
{
	class UPlayer* Player;
	class APawn* AcknowledgedPawn;
	class UInterpTrackInstDirector* ControllingDirTrackInst;
	class AHUD* MyHUD;
	class APlayerCameraManager* PlayerCameraManager;
	class APlayerCameraManager* PlayerCameraManagerClass;
	bool                                               bAutoManageActiveCameraTarget;
	unsigned char                                      UnknownData00[0x3];
	struct FRotator                                    TargetViewRotation;
	unsigned char                                      UnknownData01[0xC];
	float                                              SmoothTargetViewRotationSpeed;
	unsigned char                                      UnknownData02[0x8];
	TArray<class AActor*>                              HiddenActors;
	TArray<TWeakObjectPtr<class UPrimitiveComponent>>  HiddenPrimitiveComponents;
	unsigned char                                      UnknownData03[0x4];
	float                                              LastSpectatorStateSynchTime;
	struct FVector                                     LastSpectatorSyncLocation;
	struct FRotator                                    LastSpectatorSyncRotation;
	int                                                ClientCap;
	unsigned char                                      UnknownData04[0x4];
	class UCheatManager* CheatManager;
	class UCheatManager* CheatClass;
	class UPlayerInput* PlayerInput;
	TArray<struct FActiveForceFeedbackEffect>          ActiveForceFeedbackEffects;
	unsigned char                                      UnknownData05[0x70];
	unsigned char                                      UnknownData06 : 4;
	unsigned char                                      bPlayerIsWaiting : 1;
	unsigned char                                      UnknownData07[0x3];
	unsigned char                                      NetPlayerIndex;
	unsigned char                                      UnknownData08[0x3B];
	class UNetConnection* PendingSwapConnection;
	class UNetConnection* NetConnection;
	unsigned char                                      UnknownData09[0xC];
	float                                              InputYawScale;
	float                                              InputPitchScale;
	float                                              InputRollScale;
	unsigned char                                      bShowMouseCursor : 1;
	unsigned char                                      bEnableClickEvents : 1;
	unsigned char                                      bEnableTouchEvents : 1;
	unsigned char                                      bEnableMouseOverEvents : 1;
	unsigned char                                      bEnableTouchOverEvents : 1;
	unsigned char                                      bForceFeedbackEnabled : 1;
	unsigned char                                      UnknownData10[0x3];
	float                                              ForceFeedbackScale;
	TArray<struct FKey>                                ClickEventKeys;
	TEnumAsByte<EMouseCursor>                          DefaultMouseCursor;
	TEnumAsByte<EMouseCursor>                          CurrentMouseCursor;
	TEnumAsByte<ECollisionChannel>                     DefaultClickTraceChannel;
	TEnumAsByte<ECollisionChannel>                     CurrentClickTraceChannel;
	float                                              HitResultTraceDistance;
	uint16_t                                           SeamlessTravelCount;
	uint16_t                                           LastCompletedSeamlessTravelCount;
	unsigned char                                      UnknownData11[0x74];
	class UInputComponent* InactiveStateInputComponent;
	unsigned char                                      UnknownData12 : 2;
	unsigned char                                      bShouldPerformFullTickWhenPaused : 1;
	unsigned char                                      UnknownData13[0x17];
	class UTouchInterface* CurrentTouchInterface;
	unsigned char                                      UnknownData14[0x50];
	class ASpectatorPawn* SpectatorPawn;
	unsigned char                                      UnknownData15[0x4];
	bool                                               bIsLocalPlayerController;
	unsigned char                                      UnknownData16[0x3];
	struct FVector                                     SpawnLocation;
	unsigned char                                      UnknownData17[0xC];


};

