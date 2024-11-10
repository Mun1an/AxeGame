// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitLastMoveInput.generated.h"

class AAxeCharacterPlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLastMoveInputDelegate, FVector, LastMoveInput);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_WaitLastMoveInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FLastMoveInputDelegate OnLastMoveInputDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitLastMoveInput* CreateWaitLastMoveInputTask(
		UGameplayAbility* OwningAbility,
		AAxeCharacterPlayer* AxeCharacterPlayer);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SendDataToServer_Client();

	void OnDataReplicatedCallback_Server(const FGameplayAbilityTargetDataHandle& DataHandle,
															   FGameplayTag ActivationTag);

protected:
	TObjectPtr<AAxeCharacterPlayer> AxeCharacterPlayer;
};
