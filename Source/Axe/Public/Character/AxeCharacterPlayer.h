// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AxeCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Logging/LogMacros.h"
#include "AxeCharacterPlayer.generated.h"

class UActionCombatComponent;
class UComboActionComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AAxeCharacterPlayer : public AAxeCharacterBase, public ICombatInterface
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
		return Weapon;
	}

	virtual FORCEINLINE UStaticMeshComponent* GetWeaponSecondaryComponent() const override
	{
		return WeaponSecondary;
	}

	//
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void InitAbility();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName = FName("WeaponHandSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combat")
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSecondaryTipSocketName = FName("WeaponSecondaryHandSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combat")
	TObjectPtr<UStaticMeshComponent> WeaponSecondary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UComboActionComponent> ComboActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UActionCombatComponent> ActionCombatComponent;
};
