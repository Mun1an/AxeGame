// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_HitTrace.generated.h"

class AAxeCharacterBase;
/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_HitTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAbilityTask_HitTrace* CreateHitTraceTask(
		UGameplayAbility* OwningAbility,
		AAxeCharacterBase* Character,
		UStaticMeshComponent* TraceMeshComponent,
		FName BeginSocketName,
		FName EndSocketName,
		float Radius,
		bool bIgnoreSelf
	);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	virtual void TickTask(float DeltaTime) override;

	UPROPERTY()
	AAxeCharacterBase* Character;

	UPROPERTY()
	UStaticMeshComponent* TraceMeshComponent;

	UPROPERTY()
	FName BeginSocketName;

	UPROPERTY()
	FName EndSocketName;

	UPROPERTY()
	float Radius;

	UPROPERTY()
	bool bIgnoreSelf;

	FVector BeginSocketLocation;
	FVector EndSocketLocation;
};
