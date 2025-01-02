// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ComboMelee/ComboMelee_AxeAndShield_A3.h"

#include "Character/AxeCharacterBase.h"
#include "Interface/CombatInterface.h"

void UComboMelee_AxeAndShield_A3::SetHitTraceDefaultValue()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (IsValid(AxeCharacterOwner))
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (!HitTraceMeshComponent)
		{
			if (CombatInterface)
			{
				HitTraceMeshComponent = CombatInterface->GetWeaponSecondaryComponent();
			}
		}
	}
}
