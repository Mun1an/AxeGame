// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_OnDamageExecution.generated.h"

class UDamageCalInfo;
class UDamageExecution;
class AAxeCharacterPlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerDamageExecutionDelegate, UDamageCalInfo*, DamageCal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerDamagedExecutionDelegate, UDamageCalInfo*, DamageCal);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_OnDamageExecution : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnOwnerDamageExecutionDelegate OnDoDamageExecutionDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnOwnerDamageExecutionDelegate OnBeDamagedExecutionDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_OnDamageExecution* CreateOnDoDamageExecutionTask(
		UGameplayAbility* OwningAbility,
		AActor* DoDamageActor
	);
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_OnDamageExecution* CreateOnBeDamagedExecutionTask(
		UGameplayAbility* OwningAbility,
		AActor* BeDamagedActor
	);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	UFUNCTION()
	void OnCalDamage(UDamageCalInfo* DamageCalInfo);

	UFUNCTION()
	void InitDamageExecutionCDO();
private:
	UPROPERTY()
	TObjectPtr<AActor> DoDamageActor = nullptr;
	UPROPERTY()
	TObjectPtr<AActor> BeDamagedActor = nullptr;
	
	UPROPERTY()
	TObjectPtr<UDamageExecution> DamageExecutionCDO = nullptr;
};
