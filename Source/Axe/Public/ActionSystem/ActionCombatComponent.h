// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "ActionSystem/AxeActionComponentBase.h"
#include "ActionCombatComponent.generated.h"

enum class EDamageSpecialExpression : uint8;
class UDamageTextWidgetComponent;
enum class ELaunchCharacterDirection : uint8;
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

	// MovementSlowEffect
	FActiveGameplayEffectHandle ApplyMovementSlowEffectInAbilityUse(const float Level, const float Duration);
	bool RemoveMovementSlowEffectInAbilityUse();
	TSubclassOf<UGameplayEffect> GetMovementSlowEffectClass() const { return MovementSlowEffectClass; }

	UFUNCTION(BlueprintCallable)
	void SetCustomLaunchCharacter(float LaunchSpeed, ELaunchCharacterDirection LaunchDirection, bool bXYOverride,
	                              bool bZOverride);
	FVector GetLaunchDirectionByEnum(AActor* Actor, ELaunchCharacterDirection LaunchDirectionEnum);

	//
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetComponentClass;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float Damage, ACharacter* TargetCharacter, EDamageSpecialExpression DamageSpecialExpression, const FVector ShowLocation);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// MovementSlowEffect
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> MovementSlowEffectClass;
};
