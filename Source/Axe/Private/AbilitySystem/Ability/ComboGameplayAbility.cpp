// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ComboGameplayAbility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"

bool UComboGameplayAbility::CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const
{
	if (!Super::CanReplaceAbilityByCondition(NewAbility, Actor))
	{
		return false;
	}
	if (AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(Actor))
	{
		if (CharacterPlayer->IsLocallyControlled())
		{
			UComboActionComponent* ComboActionComponent = CharacterPlayer->GetComboActionComponent();
			bool bIsNextComboAbility = ComboActionComponent->IsNextComboAbility(NewAbility);
			bool bIsInComboSwitchWindow = ComboActionComponent->IsInComboSwitchWindow();
			return bIsNextComboAbility && bIsInComboSwitchWindow;
		}
	}
	return true;
}

void UComboGameplayAbility::Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState)
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

void UComboGameplayAbility::Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState)
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

void UComboGameplayAbility::Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState)
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

void UComboGameplayAbility::Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState)
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


void UComboGameplayAbility::Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	SetHitTraceDefaultValue();

	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
	if (!CombatInterface)
	{
		return;
	}
	AbilityTask_HitTrace = UAbilityTask_HitTrace::CreateHitTraceTask(
		this,
		AxeCharacterOwner,
		HitTraceMeshComponent,
		HitTraceStartSocketName,
		HitTraceEndSocketName,
		HitTraceRadius,
		HitTraceObjectTypes,
		bHitTraceIgnoreSelf,
		IgnoreActors
	);
	AbilityTask_HitTrace->HitTraceDelegate.AddDynamic(this, &UComboGameplayAbility::OnHitTrace);
	AbilityTask_HitTrace->ReadyForActivation();
}

void UComboGameplayAbility::Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner->IsLocallyControlled())
	{
		if (IsValid(AbilityTask_HitTrace))
		{
			AbilityTask_HitTrace->EndTask();
		}
	}
}

void UComboGameplayAbility::SetHitTraceDefaultValue()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (IsValid(AxeCharacterOwner))
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (!HitTraceMeshComponent)
		{
			if (CombatInterface)
			{
				HitTraceMeshComponent = CombatInterface->GetWeaponComponent();
			}
		}
	}
}

void UComboGameplayAbility::CreateHitParticle(const FHitResult& HitResult)
{
	// fixme 同时攻击多个目标时
	if (WeaponHit_GC_Tag.IsValid())
	{
		FGameplayCueParameters Parameters;
		Parameters.Location = HitResult.ImpactPoint;
		K2_AddGameplayCueWithParams(WeaponHit_GC_Tag, Parameters, true);
	}
}

void UComboGameplayAbility::OnHitTrace(const FHitResult& HitResults)
{
	OnHitTraceBP(HitResults);

	if (IsFirstHitTarget(HitResults.GetActor()))
	{
		// ApplyDamage
		if (HasAuthority(&CurrentActivationInfo))
		{
			AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(HitResults.GetActor());
			ApplyDamage(AxeCharacterBase);	
		}

		SetActiveMontagePauseFrame(0.08, 0.1);
		//
		if (IsLocallyControlled())
		{
			ShakeCamera();
		}
		CreateHitParticle(HitResults);
	}
}

bool UComboGameplayAbility::IsFirstHitTarget(AActor* Target)
{
	if (HasHitTargetSet.Contains(Target))
	{
		return false;
	}
	HasHitTargetSet.Add(Target);
	return true;
}
