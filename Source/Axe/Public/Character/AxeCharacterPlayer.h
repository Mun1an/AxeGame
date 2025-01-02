// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AxeCharacterBase.h"
#include "Interface/CombatInterface.h"
#include "Interface/InventoryInterface.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "AxeCharacterPlayer.generated.h"

class UWeaponTypeDataAsset;
enum class EAxeModularCharacterSM : uint8;
class UModularCharacterComponent;
class UInventoryComponent;
class UActionCombatComponent;
class UComboActionComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AAxeCharacterPlayer : public AAxeCharacterBase, public ICombatInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	AAxeCharacterPlayer();

	FORCEINLINE USpringArmComponent* GetCameraSpringArm() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ICombatInterface
	virtual FORCEINLINE UComboActionComponent* GetComboActionComponent() const override
	{
		return ComboActionComponent;
	}

	virtual FORCEINLINE UActionCombatComponent* GetActionCombatComponent() const override
	{
		return ActionCombatComponent;
	}

	virtual FORCEINLINE UStaticMeshComponent* GetWeaponComponent() const override
	{
		return WeaponSMComponent;
	}

	virtual FORCEINLINE UStaticMeshComponent* GetWeaponSecondaryComponent() const override
	{
		return WeaponSecondarySMComponent;
	}

	virtual FORCEINLINE UInventoryComponent* GetInventoryComponent() const override
	{
		return InventoryComponent;
	}

	FORCEINLINE UModularCharacterComponent* GetModularCharacterComponent() const
	{
		return ModularCharacterComponent;
	}

	//
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	USkeletalMeshComponent* GetRetargetCharacterMesh() const { return RetargetCharacterMesh; }

	TMap<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>> GetModularSkeletalMeshComponentMap() const
	{
		return ModularSkeletalMeshComponentMap;
	}

	// Anim
	UFUNCTION(BlueprintCallable)
	void SetLinkedAnimLayerClass(TSubclassOf<UAnimInstance>& InAnimInstanceClass);

	// Weapon
	void GetCurrentWeaponType(EAxePlayerWeaponType& OutWeaponType) const { OutWeaponType = CurrentWeaponType; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeaponType(EAxePlayerWeaponType InWeaponType);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void InitAbility();
	void InitInventory();

	UFUNCTION()
	void OnCurrentWeaponTypeChanged(EAxePlayerWeaponType NewWeaponType, EAxePlayerWeaponType OldWeaponType);
	UFUNCTION()
	void OnLinkedAnimLayerClassChanged();
	UFUNCTION()
	void OnEquipmentItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, UItemInstance* OldItemInstance,
	                            FGameplayTagContainer SlotTags);

	// Anim
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> LinkedAnimLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TMap<EAxePlayerWeaponType, UWeaponTypeDataAsset*> WeaponTypeDataAssetMap;

	// Weapon
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EAxePlayerWeaponType CurrentWeaponType = EAxePlayerWeaponType::None;

private:
	void HandleModularSkeletalMeshComponent(TObjectPtr<USkeletalMeshComponent>& SMComp, FName CompName,
	                                        EAxeModularCharacterSM SMEnum);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName = FName("WeaponHandSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> RetargetCharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combat")
	TObjectPtr<UStaticMeshComponent> WeaponSMComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSecondaryTipSocketName = FName("WeaponSecondaryHandSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combat")
	TObjectPtr<UStaticMeshComponent> WeaponSecondarySMComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UComboActionComponent> ComboActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UActionCombatComponent> ActionCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UModularCharacterComponent> ModularCharacterComponent;

	// ModularSkeletalMeshComponent

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_Head;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_Head_NoElements;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_Eyebrow;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_FacialHair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_Torso;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_ArmUpperRight;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_ArmUpperLeft;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_ArmLowerRight;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_ArmLowerLeft;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_HandRight;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_HandLeft;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_Hips;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_LegRight;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_Gender_LegLeft;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HeadCoverings_NoHair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HeadCoverings_BaseHair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HeadCoverings_NoFacialHair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_Hair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HeadAttachmentHair;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HeadAttachmentHelmet;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_ChestAttachment;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_BackAttachment;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_ShoulderAttachment_Right;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_ShoulderAttachment_Left;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_ElbowAttachment_Right;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_ElbowAttachment_Left;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_HipsAttachment;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_KneeAttachment_Right;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_KneeAttachment_Left;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_All_Extra;

	TMap<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>> ModularSkeletalMeshComponentMap;
};
