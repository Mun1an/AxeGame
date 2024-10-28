// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "ActionSystem/AxeActionComponentBase.h"
#include "ActionCombatComponent.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AXE_API UActionCombatComponent : public UAxeActionComponentBase
{
	GENERATED_BODY()

public:
	UActionCombatComponent();


	// Hit Trace
	void BeginHitTrace();

	// MovementSlowEffect
	FActiveGameplayEffectHandle ApplyMovementSlowEffectInAbilityUse(const float Level, const float Duration);
	void RemoveMovementSlowEffectInAbilityUse();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> MovementSlowEffectClass;
	FActiveGameplayEffectHandle MovementSlowEffectHandle;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
