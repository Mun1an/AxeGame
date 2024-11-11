// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeDamageGameplayAbility.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "ComboGameplayAbility.generated.h"

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
	// IComboAbilityInterface
	virtual void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;

	// IHitTraceInterface 
	virtual void Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void SetHitTraceDefaultValue();

	//
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHit_GC_Tag;

	UFUNCTION()
	void CreateHitParticle(const FHitResult& HitResult);

	UFUNCTION()
	virtual void OnHitTrace(const FHitResult& HitResults) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitTraceBP(const FHitResult HitResult);

protected:
	// HitTrace
	UPROPERTY()
	UStaticMeshComponent* HitTraceMeshComponent;

	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceStartSocketName = FName("Bottom");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceEndSocketName = FName("Top");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	float HitTraceRadius = 15.f;

	UPROPERTY(EditAnywhere, Category="HitTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitTraceObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_Pawn)};

	UPROPERTY(EditAnywhere, Category="HitTrace")
	bool bHitTraceIgnoreSelf = true;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	//
	UPROPERTY()
	UAbilityTask_HitTrace* AbilityTask_HitTrace;

	UFUNCTION(BlueprintCallable)
	bool IsFirstHitTarget(AActor* Target);
	UPROPERTY()
	TSet<AActor*> HasHitTargetSet;
};
