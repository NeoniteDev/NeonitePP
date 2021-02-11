#pragma once

class Player
{
public:
	UObject* Controller;
	UObject* Pawn;
	UObject* Mesh;
	UObject* AnimInstance;;

	void UpdatePlayerController()
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder LocalPlayer = EngineFinder.Find(XOR(L"GameInstance")).Find(XOR(L"LocalPlayers"));

		ObjectFinder PlayerControllerFinder = LocalPlayer.Find(XOR(L"PlayerController"));
		this->Controller = PlayerControllerFinder.GetObj();
	}

	void UpdateMesh()
	{
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));

		ObjectFinder MeshFinder = PawnFinder.Find(XOR(L"Mesh"));
		this->Mesh = MeshFinder.GetObj();
	}

	void UpdateAnimInstance()
	{
		if (!this->Mesh || !Util::IsBadReadPtr(this->Mesh))
		{
			this->UpdateMesh();
		}

		const auto FUNC_GetAnimInstance = FindObject<UFunction*>(XOR(L"Function /Script/Engine.SkeletalMeshComponent:GetAnimInstance"));

		USkeletalMeshComponent_GetAnimInstance_Params GetAnimInstance_Params;

		ProcessEvent(this->Mesh, FUNC_GetAnimInstance, &GetAnimInstance_Params);

		this->AnimInstance = GetAnimInstance_Params.ReturnValue;
	}

	void Respawn()
	{
		if (this->Pawn)
		{
			this->Summon(L"PlayerPawn_Athena_C");
			this->Pawn = ObjectFinder::FindActor(L"PlayerPawn_Athena_C");

			if (this->Pawn)
			{
				this->Possess();
				this->ShowSkin();
				this->UpdateAnimInstance();
			}
		}
	}

	void Summon(const wchar_t* ClassToSummon)
	{
		if (!this->Controller)
		{
			UpdatePlayerController();
		}

		ObjectFinder PlayerControllerFinder = ObjectFinder::EntryPoint(uintptr_t(this->Controller));

		ObjectFinder CheatManagerFinder = PlayerControllerFinder.Find(XOR(L"CheatManager"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CheatManager:Summon"));

		const FString ClassName = ClassToSummon;

		UCheatManager_Summon_Params params;
		params.ClassName = ClassName;

		ProcessEvent(CheatManagerFinder.GetObj(), fn, &params);
		printf("\n[Neoroyale] %ls was summoned!.\n", ClassToSummon);
	}

	void Possess()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Controller:Possess"));

		AController_Possess_Params params;
		params.InPawn = this->Pawn;

		ProcessEvent(this->Controller, fn, &params);
		printf("\n[Neoroyale] PlayerPawn was possessed!.\n");
	}

	auto StartSkydiving(float height)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawnAthena:TeleportToSkyDive"));

		AFortPlayerPawnAthena_TeleportToSkyDive_Params params;
		params.HeightAboveGround = height;

		ProcessEvent(this->Pawn, fn, &params);
		printf("\nSkydiving!, Redeploying at %fm.\n", height);
	}

	auto IsJumpProvidingForce()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:IsJumpProvidingForce"));

		ACharacter_IsJumpProvidingForce_Params params;

		ProcessEvent(this->Pawn, fn, &params);

		return params.ReturnValue;
	}

	auto StopMontageIfEmote() -> bool
	{
		if (!this->Mesh || !this->AnimInstance || !Util::IsBadReadPtr(this->Mesh) || !Util::IsBadReadPtr(this->AnimInstance))
		{
			this->UpdateMesh();
			this->UpdateAnimInstance();
		}

		const auto FUNC_GetCurrentActiveMontage = FindObject<UFunction*>(XOR(L"Function /Script/Engine.AnimInstance:GetCurrentActiveMontage"));

		UAnimInstance_GetCurrentActiveMontage_Params GetCurrentActiveMontage_Params;

		ProcessEvent(this->AnimInstance, FUNC_GetCurrentActiveMontage, &GetCurrentActiveMontage_Params);

		const auto CurrentPlayingMontage = GetCurrentActiveMontage_Params.ReturnValue;

		if (CurrentPlayingMontage && GetObjectFirstName(CurrentPlayingMontage).starts_with(XOR(L"Emote_")))
		{
			const auto FUNC_Montage_Stop = FindObject<UFunction*>(XOR(L"Function /Script/Engine.AnimInstance:Montage_Stop"));

			UAnimInstance_Montage_Stop_Params Montage_Stop_Params;
			Montage_Stop_Params.InBlendOutTime = 1;
			Montage_Stop_Params.Montage = CurrentPlayingMontage;

			ProcessEvent(this->AnimInstance, FUNC_Montage_Stop, &Montage_Stop_Params);
		}
	}

	auto IsSkydiving()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:IsSkydiving"));

		ACharacter_IsSkydiving_Params params;

		ProcessEvent(this->Pawn, fn, &params);

		return params.ReturnValue;
	}

	auto IsParachuteOpen()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:IsParachuteOpen"));

		ACharacter_IsParachuteOpen_Params params;

		ProcessEvent(this->Pawn, fn, &params);

		return params.ReturnValue;
	}

	auto IsParachuteForcedOpen()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:IsParachuteForcedOpen"));

		ACharacter_IsParachuteForcedOpen_Params params;

		ProcessEvent(this->Pawn, fn, &params);

		return params.ReturnValue;
	}

	auto Jump()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Character:Jump"));

		Empty_Params params;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto SetSkeletalMesh()
	{
		if (!this->Mesh || !Util::IsBadReadPtr(this->Mesh))
		{
			this->UpdateMesh();
		}

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.SkinnedMeshComponent:SetSkeletalMesh"));

		const auto Mesh = FindObject<UObject*>(
			XOR(L"SkeletalMesh /Game/Characters/Player/Male/Medium/Base/SK_M_MALE_Base.SK_M_MALE_Base"));

		if (Mesh)
		{
			USkinnedMeshComponent_SetSkeletalMesh_Params params;
			params.NewMesh = Mesh;
			params.bReinitPose = false;

			ProcessEvent(this->Mesh, fn, &params);
		}
	}

	void Thanos()
	{
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));
		ObjectFinder PlayerStateFinder = PawnFinder.Find(XOR(L"PlayerState"));
		ObjectFinder HeroTypeFinder = PlayerStateFinder.Find(XOR(L"HeroType"));

		UObject*& pcHeroType = reinterpret_cast<UObject*&>(HeroTypeFinder.GetObj());

		const auto Thanos = FindObject<UObject*>(XOR(L"FortHeroType /Game/Athena/Heroes/Dev_TestAsset_HID_M_XL.Dev_TestAsset_HID_M_XL"));

		pcHeroType = Thanos;

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerState:OnRep_HeroType"));

		ProcessEvent(PlayerStateFinder.GetObj(), fn, nullptr);
	}

	void ShowSkin()
	{
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));
		ObjectFinder PlayerStateFinder = PawnFinder.Find(XOR(L"PlayerState"));

		const auto KismetLib = FindObject<UObject*>(XOR(L"FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortKismetLibrary:UpdatePlayerCustomCharacterPartsVisualization"));

		UFortKismetLibrary_UpdatePlayerCustomCharacterPartsVisualization_Params params;
		params.PlayerState = PlayerStateFinder.GetObj();

		ProcessEvent(KismetLib, fn, &params);
		printf("\nCharacter parts should be visiable now!.\n");
	}

	void CopySkinFromPawn(UObject* PawnToCopyFrom)
	{
		struct UFortKismetLibrary_ApplyCharacterCosmetics_Params
		{
			UObject* WorldContextObject;
			UObject* CharacterParts;
			UObject* PlayerState;
			bool bSuccess;
		};

		struct UFortHero
		{
			struct FString hero_name;
			TArray<struct FFortSavedModeLoadout> mode_loadouts;
			bool Refundable;
			unsigned char UnknownData00[0x7];
			TArray<struct FMcpVariantReader> OutfitVariants;
			TArray<struct FMcpVariantReader> BackblingVariants;
			TArray<class UFortHeroSpecialization*> Specializations;
			TArray<class UFortAbilityKit*> SpecializationAbilityKits;
			TArray<UObject*> CharacterParts;
			unsigned char UnknownData01[0x8];
			TArray<struct FMcpVariantChannelInfo> OutfitVariantChannels;
			TArray<struct FMcpVariantChannelInfo> BackblingVariantChannels;
		};

		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");

		const auto Hero = FindObject<UObject*>(XOR(L"FortHero /Engine/Transient.FortHero_"));

		printf("\n\nFFS: %ls\n\n", GetObjectFullName(Hero).c_str());

		/*
		 
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));
		ObjectFinder PlayerStateFinder = PawnFinder.Find(XOR(L"PlayerState"));

		const auto KismetLib = FindObject<UObject*>(XOR(L"FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortKismetLibrary:ApplyCharacterCosmetics"));

		UFortKismetLibrary_ApplyCharacterCosmetics_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.CharacterParts = CharacterParts.GetObj();
		params.PlayerState = PlayerStateFinder.GetObj();

		ProcessEvent(KismetLib, fn, &params);
		printf("\nCharacter parts should be visiable now!.\n");

		*/
	}

	auto EquipWeapon(const wchar_t* weaponname, int guid = rand())
	{
		FGuid GUID;
		GUID.A = guid;
		GUID.B = guid;
		GUID.C = guid;
		GUID.D = guid;

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:EquipWeaponDefinition"));

		std::wstring WeaponName = weaponname;

		std::wstring name = WeaponName + L"." + WeaponName;

		auto WeaponData = FindObject<UObject*>(name.c_str(), true);

		if (WeaponData && !Util::IsBadReadPtr(WeaponData))
		{
			std::wstring objectName = GetObjectFullName(WeaponData);

			if (objectName.starts_with(L"AthenaGadget"))
			{
				const auto FUN_weapondef = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortGadgetItemDefinition:GetWeaponItemDefinition"));

				UFortGadgetItemDefinition_GetWeaponItemDefinition_Params prm_ReturnValue;

				ProcessEvent(WeaponData, FUN_weapondef, &prm_ReturnValue);

				if (prm_ReturnValue.ReturnValue && !Util::IsBadReadPtr(prm_ReturnValue.ReturnValue))
				{
					WeaponData = prm_ReturnValue.ReturnValue;
				}
			}

			//weapon found equip it
			AFortPawn_EquipWeaponDefinition_Params params;
			params.WeaponData = WeaponData;
			params.ItemEntryGuid = GUID;

			ProcessEvent(this->Pawn, fn, &params);
		}
		else
		{
			MessageBoxA(nullptr, XOR("This item doesn't exist!"), XOR("Cranium"), MB_OK);
		}
	}

	auto Emote(UObject* EmoteDef)
	{
		//We grab the mesh from the pawn then use it to get the animation instance
		if (!this->Mesh || !this->AnimInstance || !Util::IsBadReadPtr(this->Mesh) || !Util::IsBadReadPtr(this->AnimInstance))
		{
			this->UpdateMesh();
			this->UpdateAnimInstance();
		}

		const auto FUNC_GetAnimInstance = FindObject<UFunction*>(XOR(L"Function /Script/Engine.SkeletalMeshComponent:GetAnimInstance"));

		USkeletalMeshComponent_GetAnimInstance_Params GetAnimInstance_Params;

		ProcessEvent(this->Mesh, FUNC_GetAnimInstance, &GetAnimInstance_Params);

		const auto AnimInstance = GetAnimInstance_Params.ReturnValue;

		if (EmoteDef && !Util::IsBadReadPtr(EmoteDef))
		{
			//Emote Def is valid, now we grab the animation montage
			const auto FUNC_GetAnimationHardReference = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortMontageItemDefinitionBase:GetAnimationHardReference"));

			UFortMontageItemDefinitionBase_GetAnimationHardReference_Params GetAnimationHardReference_Params;
			GetAnimationHardReference_Params.BodyType = EFortCustomBodyType::All;
			GetAnimationHardReference_Params.Gender = EFortCustomGender::Both;
			GetAnimationHardReference_Params.PawnContext = this->Pawn;

			ProcessEvent(EmoteDef, FUNC_GetAnimationHardReference, &GetAnimationHardReference_Params);

			const auto Animation = GetAnimationHardReference_Params.ReturnValue;

			//got the animation, now play :JAM:
			const auto FUNC_Montage_Play = FindObject<UFunction*>(XOR(L"Function /Script/Engine.AnimInstance:Montage_Play"));

			UAnimInstance_Montage_Play_Params params;
			params.MontageToPlay = Animation;
			params.InPlayRate = 1;
			params.ReturnValueType = EMontagePlayReturnType::Duration;
			params.InTimeToStartMontageAt = 1;
			params.bStopAllMontages = true;

			ProcessEvent(AnimInstance, FUNC_Montage_Play, &params);
		}
		else
		{
			MessageBoxA(nullptr, XOR("This item doesn't exist!"), XOR("Cranium"), MB_OK);
		}
	}

	auto GetLocation() -> FVector
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.Actor:K2_GetActorLocation"));

		AActor_K2_GetActorLocation_Params params;

		ProcessEvent(this->Pawn, fn, &params);

		return params.ReturnValue;
	}

	auto SetMovementMode(TEnumAsByte<EMovementMode> NewMode, unsigned char CustomMode)
	{
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));

		ObjectFinder CharMovementFinder = PawnFinder.Find(XOR(L"CharacterMovement"));

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.CharacterMovementComponent:SetMovementMode"));

		UCharacterMovementComponent_SetMovementMode_Params params;

		params.NewMovementMode = NewMode;
		params.NewCustomMode = CustomMode;

		ProcessEvent(CharMovementFinder.GetObj(), fn, &params);
	}

	auto Fly(bool bIsFlying)
	{
		TEnumAsByte<EMovementMode> NewMode;

		if (!bIsFlying) NewMode = EMovementMode::MOVE_Flying;
		else NewMode = EMovementMode::MOVE_Walking;

		SetMovementMode(NewMode, 0);
	}

	auto SetPawnGravityScale(float GravityScaleInput)
	{
		ObjectFinder PawnFinder = ObjectFinder::EntryPoint(uintptr_t(this->Pawn));

		ObjectFinder CharMovementFinder = PawnFinder.Find(XOR(L"CharacterMovement"));

		const auto GravityScaleOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/Engine.CharacterMovementComponent"), XOR(L"GravityScale"));

		float* GravityScale = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(CharMovementFinder.GetObj()) + GravityScaleOffset);

		*GravityScale = GravityScaleInput;
	}

	auto SetHealth(float SetHealthInput)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:SetHealth"));

		AFortPawn_SetHealth_Params params;

		params.NewHealthVal = SetHealthInput;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto SetShield(float SetShieldInput)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:SetShield"));

		AFortPawn_SetShield_Params params;

		params.NewShieldValue = SetShieldInput;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto SetMaxHealth(float SetMaxHealthInput)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:SetMaxHealth"));

		AFortPawn_SetMaxHealth_Params params;

		params.NewHealthVal = SetMaxHealthInput;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto SetMaxShield(float SetMaxShieldInput)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:SetMaxShield"));

		AFortPawn_SetMaxShield_Params params;

		params.NewValue = SetMaxShieldInput;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto SetMovementSpeed(float SetMovementSpeedInput)
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPawn:SetMovementSpeed"));

		AFortPawn_SetMovementSpeedMultiplier_Params params;

		params.NewMovementSpeedVal = SetMovementSpeedInput;

		ProcessEvent(this->Pawn, fn, &params);
	}

	auto ToggleInfiniteAmmo()
	{
		const auto bEnableVoiceChatPTTOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortPlayerController"), XOR(L"bEnableVoiceChatPTT"));

		// TECHNICAL EXPLINATION: (kemo) We are doing this because InfiniteAmmo bool and some other bools live in the same offset
		// the offset has 8 bits (a bitfield), bools are only one bit as it's only 0\1 so we have a struct with 8 bools to be able to edit that specific bool
		const auto PlayerControllerBools = reinterpret_cast<PlayerControllerBoolsForInfiniteAmmo*>(reinterpret_cast<uintptr_t>(this->Controller) + bEnableVoiceChatPTTOffset);

		PlayerControllerBools->bInfiniteAmmo = true;
		PlayerControllerBools->bInfiniteMagazine = true;
	}

	auto ExecuteConsoleCommand(const wchar_t* command)
	{
		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(L"World");

		const auto KismetSysLib = FindObject<UObject*>(XOR(L"KismetSystemLibrary /Script/Engine.Default__KismetSystemLibrary"));
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.KismetSystemLibrary:ExecuteConsoleCommand"));

		UKismetSystemLibrary_ExecuteConsoleCommand_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.Command = command;
		params.SpecificPlayer = this->Controller;

		ProcessEvent(KismetSysLib, fn, &params);
	}

	auto Skydive()
	{
		/*
		if (this->IsSkydiving())
		{
			const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:EndSkydiving"));

			ProcessEvent(this->Pawn, fn, nullptr);
		}

		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:BeginSkydiving"));

		AFortPlayerPawn_BeginSkydiving_Params params;
		params.bFromBus = true;

		ProcessEvent(this->Pawn, fn, &params);
		*/
		this->SetMovementMode(EMovementMode::MOVE_Custom, 4);
	}

	auto ForceOpenParachute()
	{
		/*
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerPawn:BP_ForceOpenParachute"));

		Empty_Params params;

		ProcessEvent(this->Pawn, fn, &params);
		*/
		this->SetMovementMode(EMovementMode::MOVE_Custom, 3);
	}

	auto IsInAircraft()
	{
		const auto fn = FindObject<UFunction*>(XOR(L"Function /Script/FortniteGame.FortPlayerController:IsInAircraft"));
		ACharacter_IsInAircraft_Params params;

		ProcessEvent(this->Controller, fn, &params);
		return params.ReturnValue;
	}

	auto ShowPickaxe()
	{
		const auto CosmeticLoadoutPCOffset = ObjectFinder::FindOffset(XOR(L"Class /Script/FortniteGame.FortPlayerController"), XOR(L"CosmeticLoadoutPC"));

		const auto CosmeticLoadoutPC = reinterpret_cast<FFortAthenaLoadout*>(reinterpret_cast<uintptr_t>(this->Controller) + CosmeticLoadoutPCOffset);

		if (!Util::IsBadReadPtr(CosmeticLoadoutPC))
		{
			const auto PickaxeFinder = ObjectFinder::EntryPoint(uintptr_t(CosmeticLoadoutPC->Pickaxe));

			const auto WeaponDefFinder = PickaxeFinder.Find(XOR(L"WeaponDefinition"));

			const auto Weapon = GetObjectFirstName(WeaponDefFinder.GetObj());

			this->EquipWeapon(Weapon.c_str());
		}
	}
};
