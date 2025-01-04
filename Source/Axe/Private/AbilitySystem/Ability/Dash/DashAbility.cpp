// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Dash/DashAbility.h"

#include "AbilitySystem/Ability/Defense/ShieldBlockAbility.h"
#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystem/Tasks/AbilityTask_OnDamageExecution.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/CustomNameAnimNotifyState.h"
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

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bIsInEvade = false;

	AActor* OwningActor = GetAxeCharacterOwner();
	UAbilityTask_OnDamageExecution* AbilityTask_OnDamageExecution =
		UAbilityTask_OnDamageExecution::CreateOnBeDamagedExecutionTask(this, OwningActor);
	AbilityTask_OnDamageExecution->OnBeDamagedExecutionDelegate.AddDynamic(this, &UDashAbility::OnBeDamagedCal);
	AbilityTask_OnDamageExecution->ReadyForActivation();
}

void UDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                              bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	bIsInEvade = false;
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

void UDashAbility::Ans_CustomName_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	Super::Ans_CustomName_NotifyBegin(AnimNotifyState);
	const FName CustomName = Cast<UCustomNameAnimNotifyState>(AnimNotifyState)->CustomName;
	if (CustomName == DashEvadeCustomAnsName)
	{
		// 闪避的无敌时间
		bIsInEvade = true;
	}
}

void UDashAbility::Ans_CustomName_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	Super::Ans_CustomName_NotifyEnd(AnimNotifyState);
	const FName CustomName = Cast<UCustomNameAnimNotifyState>(AnimNotifyState)->CustomName;
	if (CustomName == DashEvadeCustomAnsName)
	{
		// 闪避的无敌时间
		bIsInEvade = false;
	}
}

void UDashAbility::OnBeDamagedCal(UDamageCalInfo* DamageCalInfo)
{
	if (bIsInEvade)
	{
		DamageCalInfo->Damage = 0;
		DamageCalInfo->bIsEvaded = true;
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
