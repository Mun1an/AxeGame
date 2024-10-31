// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "ComboGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UComboGameplayAbility : public UAxeGameplayAbility, public IComboAbilityInterface,
                                      public IHitTraceAbilityInterface
{
	GENERATED_BODY()

public:
	virtual bool CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const;

	// IComboAbilityInterface
	virtual void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;

	// IHitTraceInterface 
	virtual void Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState) override;
	virtual void Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState) override;
	virtual void SetHitTraceDefaultValue();
	UFUNCTION()
	virtual void OnHitTrace(TArray<FHitResult> HitResults) override;

protected:
	// HitTrace
	UPROPERTY()
	UStaticMeshComponent* HitTraceMeshComponent;

	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceStartSocketName = FName("Top");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	FName HitTraceEndSocketName = FName("Bottom");

	UPROPERTY(EditAnywhere, Category="HitTrace")
	float HitTraceRadius = 20.f;

	UPROPERTY(EditAnywhere, Category="HitTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitTraceObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_Pawn)};

	UPROPERTY(EditAnywhere, Category="HitTrace")
	bool bHitTraceIgnoreSelf = true;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	//
	UPROPERTY()
	UAbilityTask_HitTrace* AbilityTask_HitTrace;

	void ActiveWeaponTrailParticle();
	void DeactiveWeaponTrailParticle();
};
