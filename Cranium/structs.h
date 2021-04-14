/**
 * Copyright (c) 2020-2021 Kareem Olim (Kemo)
 * All Rights Reserved. Licensed under the Neo License
 * https://neonite.dev/LICENSE.html
 */

#pragma once
#include "enums.h"
#include <set>

struct UObject;

template <class T>
struct TArray
{
	friend struct FString;

public:

	T* Data;
	int32_t Count;
	int32_t Max;

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

	int Add(UObject* NewItem)
	{
		Count = Count + 1;
		Max = Max + 1;
		Data = static_cast<UObject**>(malloc(Count * sizeof(UObject*)));
		Data[Count - 1] = NewItem;
		return Count;
	}
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
		auto length = std::wcslen(Data);

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

struct FName;

void (*FNameToString)(FName* pThis, FString& out);

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

	auto ToString()
	{
		FString temp;
		FNameToString(this, temp);

		return temp.ToWString();
	}
};

struct FText
{
	char UnknownData[0x18];
};

struct UKismetStringLibrary_Conv_StringToName_Params
{
	FString inString;
	FName ReturnValue;
};

struct TSoftObjectPtr
{
	char unknowndata[0x28];
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

	void ProcessEvent(void* fn, void* parms)
	{
		auto vtable = *reinterpret_cast<void***>(this);
		auto processEventFn = static_cast<void(*)(void*, void*, void*)>(vtable[0x44]);
		processEventFn(this, fn, parms);
	}

	bool IsA(UClass* cmp) const
	{
		if (this->Class == cmp)
		{
			return true;
		}
		return false;
	}

	std::wstring GetName()
	{
		return NamePrivate.ToString();
	}

	std::wstring GetFullName()
	{
		std::wstring temp;

		for (auto outer = Outer; outer; outer = outer->Outer)
		{
			temp = outer->GetName() + L"." + temp;
		}

		temp = reinterpret_cast<UObject*>(Class)->GetName() + L" " + temp + this->GetName();
		return temp;
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

struct ACharacter_IsSkydiving_Params
{
	bool ReturnValue;
};

struct ACharacter_IsParachuteOpen_Params
{
	bool ReturnValue;
};

struct ACharacter_IsParachuteForcedOpen_Params
{
	bool ReturnValue;
};

struct ACharacter_IsInAircraft_Params
{
	bool ReturnValue;
};

struct Empty_Params
{
};

struct UGameUserSettings_SetFullscreenMode_Params
{
	TEnumAsByte<EWindowMode> InFullscreenMode;
};

struct K2_OnMovementModeChanged_Params
{
	char PrevMovementMode;
	char NewMovementMode;
	char PrevCustomMode;
	char NewCustomMode;
};

struct UFortKismetLibrary_ApplyCharacterCosmetics_Params
{
	UObject* WorldContextObject;
	TArray<UObject*> CharacterParts;
	UObject* PlayerState;
	bool bSuccess;
};

struct PickupActor_Params
{
	UObject* PickupTarget;
	UObject* PlacementDecoItemDefinition;
};

struct APlayerController_ClientSetCameraMode_Params
{
	FName NewCamMode;
};

struct UPrimitiveComponent_SetOwnerNoSee_Params
{
	bool bNewOwnerNoSee;
};

struct AFortPlayerPawn_BeginSkydiving_Params
{
	bool bFromBus;
};

struct UCheatManager_CheatScript_Params
{
	FString ScriptName;
};

struct AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride_Params
{
	UClass* TimeOfDayManagerClass;
};

struct AFortPlayerPawnAthena_TeleportToSkyDive_Params
{
	float HeightAboveGround;
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

struct USkinnedMeshComponent_SetSkeletalMesh_Params
{
	UObject* NewMesh;
	bool bReinitPose;
};

struct UMaterialInstanceDynamic_SetTextureParameterValue_Params
{
	FName ParameterName;
	UObject* Value;
};

struct UFortKismetLibrary_UpdatePlayerCustomCharacterPartsVisualization_Params
{
	UObject* PlayerState;
};

struct FGuid
{
	int A;
	int B;
	int C;
	int D;
};

struct UCharacterMovementComponent_SetMovementMode_Params
{
	TEnumAsByte<EMovementMode> NewMovementMode;
	unsigned char NewCustomMode;
};

struct AFortPawn_EquipWeaponDefinition_Params
{
	UObject* WeaponData;
	FGuid ItemEntryGuid;
	UObject* ReturnValue;
};

struct FPointerToUberGraphFrame
{
	unsigned char UnknownData00[0x8];
};

struct AFortPawn_OnWeaponEquipped_Params
{
	UObject* NewWeapon;
	UObject* PrevWeapon;
};

struct UFortniteAutomationBlueprintLibrary_ApplyItemWrapToActor_Params
{
	UObject* wrap;
	UObject* Actor;
	EItemWrapMaterialType MaterialType;
};

struct AFortPawn_SetHealth_Params
{
	float NewHealthVal;
};

struct AFortPawn_SetShield_Params
{
	float NewShieldValue;
};

struct AFortPawn_SetMaxShield_Params
{
	float NewValue;
};

struct AFortPawn_SetMaxHealth_Params
{
	float NewHealthVal;
};

struct AFortPawn_SetMovementSpeedMultiplier_Params
{
	float NewMovementSpeedVal;
};

struct AGameMode_Say_Params
{
	struct FString Msg;
};

struct FPlaylistPropertyArray
{
	char padding[0x0120];
	UObject* BasePlaylist;
	UObject* OverridePlaylist;
};

struct PlayerControllerBoolsForInfiniteAmmo
{
	bool bEnableVoiceChatPTT : 1;
	bool bVoiceChatPTTTransmit : 1;
	bool bInfiniteAmmo : 1;
	bool bInfiniteMagazine : 1;
	bool bNoCoolDown : 1;
	bool bInfiniteDurability : 1;
	bool bUsePickers : 1;
	bool bPickerOpen : 1;
};

struct AFortGameStateAthena_OnRep_GamePhase_Params
{
	EAthenaGamePhase OldGamePhase;
};

struct UFortGadgetItemDefinition_GetWeaponItemDefinition_Params
{
	UObject* ReturnValue;
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

struct UEngine;

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

	FRotator()
		: Pitch(0),
		  Yaw(0),
		  Roll(0)
	{
	}

	FRotator(float pitch, float yaw, float roll)
		: Pitch(pitch),
		  Yaw(yaw),
		  Roll(roll)
	{
	}
};

struct FQuat
{
	float W, X, Y, Z;

	FQuat()
		: W(0),
		  X(0),
		  Y(0),
		  Z(0)
	{
	}

	FQuat(float w, float x, float y, float z)
		: W(w),
		  X(x),
		  Y(y),
		  Z(z)
	{
	}
};

struct FTransform
{
	FQuat Rotation;
	FVector Translation;
	char UnknownData_1C[0x4];
	FVector Scale3D;
	char UnknownData_2C[0x4];
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

struct FActorSpawnParameters
{
	FActorSpawnParameters(): Name(), Template(nullptr), Owner(nullptr), Instigator(nullptr), OverrideLevel(nullptr), SpawnCollisionHandlingOverride(), bRemoteOwned(0), bNoFail(0),
	                         bDeferConstruction(0),
	                         bAllowDuringConstructionScript(0),
	                         NameMode(),
	                         ObjectFlags()
	{
	}
	;


	FName Name;

	UObject* Template; //AActor

	UObject* Owner; //AActor

	UObject* Instigator; //APawn

	UObject* OverrideLevel; //ULevel

	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:

	uint8_t bRemoteOwned : 1;

public:

	bool IsRemoteOwned() const { return bRemoteOwned; }

	uint8_t bNoFail : 1;


	uint8_t bDeferConstruction : 1;

	uint8_t bAllowDuringConstructionScript : 1;


	enum class ESpawnActorNameMode : uint8_t
	{
		Required_Fatal,

		Required_ErrorAndReturnNull,

		Required_ReturnNull,

		Requested
	};


	ESpawnActorNameMode NameMode;

	EObjectFlags ObjectFlags;
};

struct FLinearColor
{
	float R;
	float G;
	float B;
	float A;

	FLinearColor()
		: R(0), G(0), B(0), A(0)
	{
	}

	FLinearColor(float r, float g, float b, float a)
		: R(r),
		  G(g),
		  B(b),
		  A(a)
	{
	}
};

struct SetBrushFromTextureParams
{
	UObject* Texture;
	bool bMatchSize;
};

struct SetInventoryPanelOverride_Params
{
	UObject* InInventoryPanelOverride;
};

struct FQosRegionInfo
{
	FText DisplayName;
	FString RegionId;
};

struct UKismetRenderingLibrary_ImportFileAsTexture2D_Params
{
	UObject* WorldContextObject;
	FString Filename;
	UObject* ReturnValue;
};

struct UFortItemDefinition_CreateTemporaryItemInstanceBP_Params
{
	int Count;
	int Level;
	UObject* ReturnValue;
};

struct ULevelStreamingDynamic_LoadLevelInstance_Params
{
	UObject* WorldContextObject;
	FString LevelName;
	FVector Location;
	FRotator Rotation;
	bool bOutSuccess;
	FString OptionalLevelNameOverride;
	UObject* ReturnValue;
};

struct AActor_K2_GetActorLocation_Params
{
	FVector ReturnValue;
};

struct AActor_ReceiveHit_Params
{
	UObject* MyComp;
	UObject* Other;
	UObject* OtherComp;
	bool bSelfMoved;
	FVector HitLocation;
};

struct AActor_K2_TeleportTo_Params
{
	FVector DestLocation;
	FRotator DestRotation;
	bool ReturnValue;
};

struct FLatentActionInfo
{
	int Linkage;
	int UUID;
	FName ExecutionFunction;
	UObject* CallbackTarget;
};

struct UGameplayStatics_LoadStreamLevel_Params
{
	UObject* WorldContextObject;
	FName LevelName;
	bool bMakeVisibleAfterLoad;
	bool bShouldBlockOnLoad;
	FLatentActionInfo LatentInfo;
};

struct GetAllActorsOfClass_Params
{
	UObject* WorldContextObject;
	UObject* ActorClass; //AActor
	TArray<UObject*> OutActors; //AActor
};

struct UFortKismetLibrary_SetTimeOfDay_Params
{
	UObject* WorldContextObject;
	float TimeOfDay;
};

struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
{
	UObject* WorldContextObject;
	FString Command;
	UObject* SpecificPlayer;
};

struct USkeletalMeshComponent_GetAnimInstance_Params
{
	UObject* ReturnValue;
};

struct UAnimInstance_GetCurrentActiveMontage_Params
{
	UObject* ReturnValue;
};


struct UAnimInstance_Montage_Stop_Params
{
	float InBlendOutTime;
	UObject* Montage;
};


struct UFortMontageItemDefinitionBase_GetAnimationHardReference_Params
{
	TEnumAsByte<EFortCustomBodyType> BodyType;
	TEnumAsByte<EFortCustomGender> Gender;
	UObject* PawnContext;
	UObject* ReturnValue;
};

struct UAnimInstance_Montage_Play_Params
{
	UObject* MontageToPlay;
	float InPlayRate;
	EMontagePlayReturnType ReturnValueType;
	float InTimeToStartMontageAt;
	bool bStopAllMontages;
	float ReturnValue;
};

struct PlayIntroAnim_Params
{
	EAthenaGamePhaseStep Step;
};

struct FFortAthenaLoadout
{
	FString BannerIconId;
	FString BannerColorId;
	UObject* SkyDiveContrail;
	UObject* Glider;
	UObject* Pickaxe;
	bool bIsDefaultCharacter;
	unsigned char UnknownData00[0x7];
	UObject* Character;
	TArray<UObject*> CharacterVariantChannels;
	bool bForceUpdateVariants;
	unsigned char UnknownData01[0x7];
	UObject* Hat;
	UObject* Backpack;
	UObject* LoadingScreen;
	UObject* BattleBus;
	UObject* VehicleDecoration;
	UObject* CallingCard;
	UObject* MapMarker;
	TArray<UObject*> Dances;
	UObject* VictoryPose;
	UObject* MusicPack;
	UObject* ItemWrapOverride;
	TArray<UObject*> ItemWraps;
	UObject* CharmOverride;
	TArray<UObject*> Charms;
	UObject* PetSkin;
};
