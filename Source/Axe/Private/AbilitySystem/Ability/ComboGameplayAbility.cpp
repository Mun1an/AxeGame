// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ComboGameplayAbility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "Item/ItemActor/WeaponEquipmentItemActor.h"

UComboGameplayAbility::UComboGameplayAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bUseClientMovement = true;
	bNeedAutoSearchTarget = true;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	WeaponTrail_GC_Tag = FAxeGameplayTags::Get().GameplayCue_WeaponTrail_Axe;
}

void UComboGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityTask_HitTrace = nullptr;
	HasHitTargetSet.Empty();
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

//
void UComboGameplayAbility::Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	SetHitTraceDefaultValue();

	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
	if (!CombatInterface)
	{
		return;
	}
	if (HitTraceMeshComponent)
	{
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

	HasHitTargetSet.Empty();

	// WeaponTrail GameplayCue
	FGameplayCueParameters GameplayCueParameters = FGameplayCueParameters();
	GameplayCueParameters.TargetAttachComponent = HitTraceMeshComponent;
	K2_AddGameplayCueWithParams(WeaponTrail_GC_Tag, GameplayCueParameters, true);
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

	// WeaponTrail GameplayCue
	K2_RemoveGameplayCue(WeaponTrail_GC_Tag);
}

void UComboGameplayAbility::SetHitTraceDefaultValue()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (IsValid(AxeCharacterOwner))
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (CombatInterface)
		{
			AWeaponEquipmentItemActor* WeaponActor = CombatInterface->GetWeaponEquipmentActor_Implementation();
			if (WeaponActor)
			{
				HitTraceMeshComponent = WeaponActor->GetHitTraceMeshComponent();
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

	// Hit Once
	if (IsFirstHitTarget(HitResults.GetActor()))
	{
		// ApplyDamage
		if (HasAuthority(&CurrentActivationInfo))
		{
			AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(HitResults.GetActor());
			ApplyDamageToTarget(AxeCharacterBase, HitResults);
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
