// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "ItemUseAbility.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class AXE_API UItemUseAbility : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo,
	                           FGameplayTagContainer* OptionalRelevantTags) override;

	UFUNCTION(BlueprintCallable)
	bool CheckCanUseItemByIndex(int32 UseItemBarIndex);

	UFUNCTION(BlueprintCallable)
	bool UseItemByIndex(int32 UseItemBarIndex);

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetOwnerInventoryComponent();
	UFUNCTION(BlueprintCallable)
	int32 GetInvSlotIndexByUseItemIndex(int32 UseItemBarIndex);

	UFUNCTION(BlueprintCallable)
	AActor* CreateItemDisplayActor(const FTransform& Transform, FName AttachSocket);

	UPROPERTY(BlueprintReadOnly)
	int32 UseItemBarSlotIndex = -1;

private:
	UPROPERTY()
	UInventoryComponent* OwnerInventoryComponent;
};
