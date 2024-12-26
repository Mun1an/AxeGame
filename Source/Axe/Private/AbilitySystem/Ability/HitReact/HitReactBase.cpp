// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/HitReact/HitReactBase.h"

#include "Character/AxeCharacterBase.h"
#include "Enum/AxeEnum.h"
#include "Net/UnrealNetwork.h"

void UHitReactBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}



ELaunchCharacterDirection UHitReactBase::GetHitDirectionMontage() const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (!AxeCharacterOwner)
	{
		return ELaunchCharacterDirection::Lc_Backward;
	}
	FVector HitLocation = HitResult.ImpactPoint;
	if (HitLocation.IsNearlyZero() && !SourceActor)
	{
		return ELaunchCharacterDirection::Lc_Backward;
	}
	if (SourceActor)
	{
		HitLocation = SourceActor->GetActorLocation();
	}

	FVector CharacterLocation = AxeCharacterOwner->GetActorLocation();
	FVector CharacterDirection = AxeCharacterOwner->GetActorRotation().Vector().GetSafeNormal();

	FVector HitDirection = (HitLocation - CharacterLocation).GetSafeNormal();
	HitDirection.Z = 0.f;

	double Dot = CharacterDirection.Dot(HitDirection);
	float Angle = FMath::Acos(Dot) * 180.f / PI;
	FVector CrossVector = CharacterDirection.Cross(HitDirection);

	if (Angle < 45.f)
	{
		return ELaunchCharacterDirection::Lc_Forward;
	}
	if (Angle < 135.f)
	{
		if (CrossVector.Z > 0.f)
		{
			return ELaunchCharacterDirection::Lc_Right;
		}
		else
		{
			return ELaunchCharacterDirection::Lc_Left;
		}
	}
	return ELaunchCharacterDirection::Lc_Backward;
}
