// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UDashAbility : public UAxeGameplayAbility, public IComboAbilityInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashMontage_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashMontage_B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashMontage_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashMontage_R;

	UFUNCTION(BlueprintCallable)
	ELaunchCharacterDirection GetDashDirectionByMovementVector(const FVector MovementVector) const;

	// IComboAbilityInterface
	virtual void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
};
