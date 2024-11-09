// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitFacingRot.generated.h"

class AAxeCharacterPlayer;

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_WaitFacingRot : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitFacingRot* CreateWaitFacingRotTask(
		UGameplayAbility* OwningAbility,
		ACharacter* Character,
		FRotator TargetRot,
		float InterpSpeed
	);
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	TObjectPtr<ACharacter> AxeCharacterBase;
	FRotator TargetRot;
	float InterpSpeed = 1.f;

	float MaxTickTime = 1.0f;
	float TotalTickTime = 0.f;
};
