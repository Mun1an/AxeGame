// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeDamageGameplayAbility.h"
#include "LeapAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API ULeapAttackAbility : public UAxeDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	float GetJumpDistanceByAutoTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetJumpDirection();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float JumpDistance = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float JumpHeight = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float JumpDuration = 0.6f;
};
