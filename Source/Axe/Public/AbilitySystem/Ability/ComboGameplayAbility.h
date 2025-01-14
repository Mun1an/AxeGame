// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeDamageGameplayAbility.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "ComboGameplayAbility.generated.h"

class AWeaponEquipmentItemActor;
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class AXE_API UComboGameplayAbility : public UAxeDamageGameplayAbility, public IComboAbilityInterface,
                                      public IHitTraceAbilityInterface
{
	GENERATED_BODY()

public:
	UComboGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
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

	// IHitTraceInterface 
	virtual void Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	AWeaponEquipmentItemActor* GetHitTraceWeaponActorByEnum(EHitTraceWeaponHandIndex HitTraceWeaponHand);

	//
	virtual void HandleTryActivateAbilityClientCDO(FGameplayAbilitySpecHandle& AbilitySpecHandle,
	                                               const FGameplayTag AbilityInputTag, AActor* AbilityActor) override;

	//
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHit_GC_Tag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponWave_GC_Tag;

	UFUNCTION()
	void CreateHitParticle(const FHitResult& HitResult);

	UFUNCTION()
	virtual void OnHitTrace(const FHitResult& HitResults);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitTraceBP(const FHitResult HitResult);

protected:
	// HitTrace
	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceDefaultStartSocketName = FName("Bottom");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceDefaultEndSocketName = FName("Top");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	float HitTraceDefaultRadius = 15.f;

	UPROPERTY(EditAnywhere, Category="HitTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitTraceDefaultObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_Pawn)};

	UPROPERTY(EditAnywhere, Category="HitTrace")
	bool bHitTraceDefaultIgnoreSelf = true;

	UPROPERTY(BlueprintReadWrite, Category="HitTrace")
	TArray<AActor*> HitTraceDefaultIgnoreActors = {};

	//
	UPROPERTY()
	TMap<UAnimNotifyState*, UAbilityTask_HitTrace*> AnimNotifyStateToHitTraceTaskMap;
};
