// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "HitReactBase.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UHitReactBase : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetHitResult(const FHitResult& InHitResult)
	{
		HitResult = InHitResult;
	}

protected:
	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReact")
	UAnimMontage* HitReactMontage_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReact")
	UAnimMontage* HitReactMontage_B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReact")
	UAnimMontage* HitReactMontage_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReact")
	UAnimMontage* HitReactMontage_R;

	UFUNCTION(BlueprintCallable)
	ELaunchCharacterDirection GetHitDirectionMontageByHitResult(const FHitResult& InHitResult) const;
};
