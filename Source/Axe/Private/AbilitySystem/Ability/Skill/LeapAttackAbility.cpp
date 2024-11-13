// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Skill/LeapAttackAbility.h"

#include "Character/AxeCharacterBase.h"

float ULeapAttackAbility::GetJumpDistanceByAutoTarget()
{
	AActor* Target = GetOrFindAutoTargetActor();
	if (!Target)
	{
		return JumpDistance;
	}
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector OwnerLocation = GetAxeCharacterOwner()->GetActorLocation();
	const float Dist = (OwnerLocation - TargetLocation).Size();
	return Dist;
}

FVector ULeapAttackAbility::GetJumpDirection()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AActor* Target = GetOrFindAutoTargetActor();
	if (!Target)
	{
		return AxeCharacterOwner->GetActorForwardVector();
	}
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector OwnerLocation = AxeCharacterOwner->GetActorLocation();
	const FVector Dir = (TargetLocation - OwnerLocation).GetSafeNormal();
	return Dir;
}
