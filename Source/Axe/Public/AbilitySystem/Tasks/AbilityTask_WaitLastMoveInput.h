// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitLastMoveInput.generated.h"

class AAxeCharacterPlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLastMoveInputDelegate);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_WaitLastMoveInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	FLastMoveInputDelegate OnLastMoveInputDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitLastMoveInput* CreateWaitLastMoveInputTask(
		UGameplayAbility* OwningAbility,
		ACharacter* AxeCharacterPlayer);
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	TObjectPtr<ACharacter> AxeCharacterPlayer;
};
