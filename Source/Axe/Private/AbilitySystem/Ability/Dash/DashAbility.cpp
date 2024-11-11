// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Dash/DashAbility.h"

#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Enum/AxeEnum.h"

ELaunchCharacterDirection UDashAbility::GetDashDirectionByMovementVector(const FVector MovementVector) const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (!AxeCharacterOwner || MovementVector.IsNearlyZero())
	{
		return ELaunchCharacterDirection::Lc_Backward;
	}
	FVector CharacterDirection = AxeCharacterOwner->GetActorRotation().Vector().GetSafeNormal();

	double Dot = CharacterDirection.Dot(MovementVector);
	float Angle = FMath::Acos(Dot) * 180.f / PI;
	FVector CrossVector = CharacterDirection.Cross(MovementVector);

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

void UDashAbility::Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		UComboActionComponent* ComboActionComponent = GetComboActionComponent();
		if (ComboActionComponent)
		{
			ComboActionComponent->AnsComboInputCacheStart();
		}
	}
}

void UDashAbility::Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		UComboActionComponent* ComboActionComponent = GetComboActionComponent();
		if (ComboActionComponent)
		{
			ComboActionComponent->AnsComboInputCacheEnd();
		}
	}
}

void UDashAbility::Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		UComboActionComponent* ComboActionComponent = GetComboActionComponent();
		if (ComboActionComponent)
		{
			ComboActionComponent->AnsComboSwitchWindowStart(AnimNotifyState);
		}
	}
}

void UDashAbility::Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		UComboActionComponent* ComboActionComponent = GetComboActionComponent();
		if (ComboActionComponent)
		{
			ComboActionComponent->AnsComboSwitchWindowEnd(AnimNotifyState);
		}
	}
}