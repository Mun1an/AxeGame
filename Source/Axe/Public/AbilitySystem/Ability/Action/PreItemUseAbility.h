// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "PreItemUseAbility.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class AXE_API UPreItemUseAbility : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void HandleTryActivateAbilityClientCDO(FGameplayAbilitySpecHandle& AbilitySpecHandle,
	                                               const FGameplayTag AbilityInputTag, AActor* AbilityActor) override;
	UFUNCTION(BlueprintCallable)
	int32 GetInvSlotIndexByUseItemIndex(int32 UseItemBarIndex);

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetOwnerInventoryComponent();

	UInventoryComponent* GetInventoryComponent(AActor* Actor);

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	int32 GetUseItemBarIndexByAbilityInputTag(const FGameplayTag AbilityInputTag);

	bool ActivateExactUseAbility(FGameplayTag AbilityInputTag);

private:
	UPROPERTY()
	UInventoryComponent* OwnerInventoryComponent;
};
