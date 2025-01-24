// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/EffectAbility/StunAbility.h"

UStunAbility::UStunAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
