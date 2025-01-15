// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AxeCharacterBase.h"
#include "Interface/CombatInterface.h"
#include "Interface/InventoryInterface.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "AxeCharacterPlayer.generated.h"

class AWeaponEquipmentItemActor;
class UWeaponDataAsset;
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
	virtual UComboActionComponent* GetComboActionComponent_Implementation() const override
	{
		return ComboActionComponent;
	}

	virtual UActionCombatComponent* GetActionCombatComponent_Implementation() const override
	{
		return ActionCombatComponent;
	}

	virtual AWeaponEquipmentItemActor* GetWeaponEquipmentActor_Implementation() const override
	{
		return WeaponEquipmentActor;
	}

	virtual AWeaponEquipmentItemActor* GetWeaponEquipmentActorSecondary_Implementation() const override
	{
		return WeaponEquipmentActorSecondary;
	}

	UFUNCTION(BlueprintCallable)
	void SetWeaponEquipmentActor(AWeaponEquipmentItemActor* InWeaponEquipmentActor)
	{
		WeaponEquipmentActor = InWeaponEquipmentActor;
	}

	UFUNCTION(BlueprintCallable)
	void SetWeaponEquipmentActorSecondary(AWeaponEquipmentItemActor* InWeaponEquipmentActor)
	{
		WeaponEquipmentActorSecondary = InWeaponEquipmentActor;
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

	TMap<EAxeModularCharacterSM, TObjectPtr<USkeletalMeshComponent>> GetModularSkeletalMeshComponentMap() const
	{
		return ModularSkeletalMeshComponentMap;
	}

	// Anim
	UFUNCTION(BlueprintCallable)
	void SetLinkedAnimLayerClass(TSubclassOf<UAnimInstance>& InAnimInstanceClass);
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UAnimInstance> GetDefaultLinkedAnimLayerClass() const { return DefaultLinkedAnimLayerClass; }

	UFUNCTION(BlueprintCallable)
	void ResetLinkedAnimLayerClass();

	// Weapon

	// Level
	virtual int32 GetCharacterLevel() const override;
	virtual void SetCharacterLevel(int32 NewCharacterLevel) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void InitAbility();
	void InitInventory();

	UFUNCTION()
	void OnLinkedAnimLayerClassChanged();

	UFUNCTION()
	void OnServerLevelUp(int32 NewValue, int32 OldValue);

	// Anim
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultLinkedAnimLayerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> LinkedAnimLayerClass;

	// Weapon
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponEquipmentItemActor* WeaponEquipmentActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponEquipmentItemActor* WeaponEquipmentActorSecondary = nullptr;

private:
	void HandleModularSkeletalMeshComponent(TObjectPtr<USkeletalMeshComponent>& SMComp, FName CompName,
	                                        EAxeModularCharacterSM SMEnum);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> RetargetCharacterMesh;

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

	TMap<EAxeModularCharacterSM, TObjectPtr<USkeletalMeshComponent>> ModularSkeletalMeshComponentMap;
};
