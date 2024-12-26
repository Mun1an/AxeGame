// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN/AbilitySkillStageAnimNotify.h"

#include "AbilitySystem/Ability/AxeGameplayAbility.h"

UAbilitySkillStageAnimNotify::UAbilitySkillStageAnimNotify(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::White;
#endif

	AbilitySkillStage = EAbilitySkillStage::ASS_FrontSwing;
}
