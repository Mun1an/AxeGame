// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_HitTrace.generated.h"

class AAxeCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitTraceDelegate, const FHitResult&, HitResults);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_HitTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	FHitTraceDelegate HitTraceDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_HitTrace* CreateHitTraceTask(
		UGameplayAbility* OwningAbility,
		AAxeCharacterBase* Character,
		UStaticMeshComponent* TraceMeshComponent,
		FName BeginSocketName,
		FName EndSocketName,
		float Radius,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		bool bIgnoreSelf,
		const TArray<AActor*>& IgnoreActors
	);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	virtual void TickTask(float DeltaTime) override;

	void SendTargetDataToServer_Client();

	void OnHitDataReplicatedCallback_Server(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
	
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
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;

	UPROPERTY()
	bool bIgnoreSelf;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	FVector BeginSocketLocation;
	FVector EndSocketLocation;
};
