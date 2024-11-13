// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeProjectileAbility.h"

#include "Actor/Projectile/AxeProjectile.h"
#include "Character/AxeCharacterBase.h"


AAxeProjectile* UAxeProjectileAbility::SpawnProjectile(const FTransform& SpawnTransform)
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return nullptr;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.DamageEffectClass = DamageEffectClass;
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DamageValue = GetTotalDamage();
	AAxeProjectile* SpawnProjectile = AAxeProjectile::SpawnProjectile(ProjectileClass, AxeCharacterOwner,
	                                                                  DamageEffectParams, SpawnTransform);
	return SpawnProjectile;
}

FVector UAxeProjectileAbility::GetDirectionByTargetLocation()
{
	AActor* Target = GetOrFindAutoTargetActor();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (!Target)
	{
		const FRotator ActorRotation = AxeCharacterOwner->GetActorRotation();
		return ActorRotation.Vector();
	}
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector ActorLocation = AxeCharacterOwner->GetActorLocation();

	return (TargetLocation - ActorLocation).GetSafeNormal();
}
