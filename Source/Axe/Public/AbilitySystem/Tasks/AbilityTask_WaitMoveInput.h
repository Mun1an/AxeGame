// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitMoveInput.generated.h"

class AAxeCharacterPlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveInputDelegate, FVector, LastMoveInput);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_WaitMoveInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMoveInputDelegate OnMoveInputDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitMoveInput* CreateWaitMoveInputTask(
		UGameplayAbility* OwningAbility,
		AAxeCharacterPlayer* AxeCharacterPlayer);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	TObjectPtr<AAxeCharacterPlayer> AxeCharacterPlayer;
};
