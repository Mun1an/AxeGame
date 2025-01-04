// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "DashAbility.generated.h"

class UDamageCalInfo;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FName DashEvadeCustomAnsName;

	UFUNCTION(BlueprintCallable)
	ELaunchCharacterDirection GetDashDirectionByMovementVector(const FVector MovementVector) const;
	//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	// IComboAbilityInterface
	virtual void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	//
	virtual void Ans_CustomName_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_CustomName_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;

protected:
	UFUNCTION()
	void OnBeDamagedCal(UDamageCalInfo* DamageCalInfo);

	UPROPERTY(BlueprintReadWrite)
	bool bIsInEvade = false;
};
