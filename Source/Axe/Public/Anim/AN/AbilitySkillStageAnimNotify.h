// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AN/AxeAnimNotifyBase.h"
#include "AbilitySkillStageAnimNotify.generated.h"

enum class EAbilitySkillStage : uint8;
/**
 * 
 */
UCLASS()
class AXE_API UAbilitySkillStageAnimNotify : public UAxeAnimNotifyBase
{
	GENERATED_BODY()

public:
	UAbilitySkillStageAnimNotify(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	EAbilitySkillStage GetAbilitySkillStage() const { return AbilitySkillStage; }

protected:
	UPROPERTY(EditAnywhere)
	EAbilitySkillStage AbilitySkillStage;
};
