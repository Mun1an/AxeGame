// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeDamageGameplayAbility.h"
#include "AxeProjectileAbility.generated.h"

class AAxeProjectile;
/**
 * 
 */
UCLASS()
class AXE_API UAxeProjectileAbility : public UAxeDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAxeProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual AAxeProjectile* SpawnProjectile(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	FVector GetDirectionByTargetLocation();
};
