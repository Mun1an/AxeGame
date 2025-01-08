// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ComboGameplayAbility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/HitTraceAnimNotifyState.h"
#include "Character/AxeCharacterPlayer.h"
#include "Item/ItemActor/WeaponEquipmentItemActor.h"

UComboGameplayAbility::UComboGameplayAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bUseClientMovement = true;
	bNeedAutoSearchTarget = true;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	WeaponWave_GC_Tag = FAxeGameplayTags::Get().GameplayCue_WeaponWave_Axe;

	AnimNotifyStateToHitTraceTaskMap = TMap<UAnimNotifyState*, UAbilityTask_HitTrace*>();
}

void UComboGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AnimNotifyStateToHitTraceTaskMap.Empty();
}

void UComboGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(AxeCharacterOwner);

	// Weapon Trail
	if (AWeaponEquipmentItemActor* WeaponActor = AxeCharacterPlayer->
		GetWeaponEquipmentActor_Implementation())
	{
		WeaponActor->ResetWeaponTrail();
	}
	if (AWeaponEquipmentItemActor* WeaponSecondaryActor = AxeCharacterPlayer->
		GetWeaponEquipmentActorSecondary_Implementation())
	{
		WeaponSecondaryActor->ResetWeaponTrail();
	}
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
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
	if (!CombatInterface)
	{
		return;
	}
	UHitTraceAnimNotifyState* HitTraceAnimNotifyState = Cast<UHitTraceAnimNotifyState>(AnimNotifyState);
	if (!HitTraceAnimNotifyState)
	{
		return;
	}
	AWeaponEquipmentItemActor* WeaponActor = GetHitTraceWeaponActorByEnum(HitTraceAnimNotifyState->HitTraceWeaponHand);
	if (!WeaponActor)
	{
		return;
	}

	UMeshComponent* HitTraceMeshComponent = WeaponActor->GetHitTraceMeshComponent();
	UAbilityTask_HitTrace* AbilityTask_HitTrace = UAbilityTask_HitTrace::CreateHitTraceTask(
		this,
		AxeCharacterOwner,
		HitTraceMeshComponent,
		HitTraceDefaultStartSocketName,
		HitTraceDefaultEndSocketName,
		HitTraceDefaultRadius,
		HitTraceDefaultObjectTypes,
		bHitTraceDefaultIgnoreSelf,
		HitTraceDefaultIgnoreActors
	);
	AbilityTask_HitTrace->HitTraceDelegate.AddDynamic(this, &UComboGameplayAbility::OnHitTrace);
	AbilityTask_HitTrace->ReadyForActivation();

	AnimNotifyStateToHitTraceTaskMap.Add(AnimNotifyState, AbilityTask_HitTrace);

	// WeaponTrail GameplayCue
	K2_AddGameplayCueWithParams(WeaponWave_GC_Tag, FGameplayCueParameters(), true);

	WeaponActor->SetWeaponTrail(true);
}

void UComboGameplayAbility::Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	// AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	// if (AxeCharacterOwner->IsLocallyControlled())
	// {
	// 	
	// }
	UAbilityTask_HitTrace* AbilityTask_HitTrace = AnimNotifyStateToHitTraceTaskMap.FindRef(AnimNotifyState);
	if (IsValid(AbilityTask_HitTrace))
	{
		AbilityTask_HitTrace->EndTask();
		AnimNotifyStateToHitTraceTaskMap.Remove(AnimNotifyState);
	}

	UHitTraceAnimNotifyState* HitTraceAnimNotifyState = Cast<UHitTraceAnimNotifyState>(AnimNotifyState);
	if (HitTraceAnimNotifyState)
	{
		AWeaponEquipmentItemActor* WeaponActor = GetHitTraceWeaponActorByEnum(
			HitTraceAnimNotifyState->HitTraceWeaponHand);
		WeaponActor->SetWeaponTrail(false);
	}
}

AWeaponEquipmentItemActor* UComboGameplayAbility::GetHitTraceWeaponActorByEnum(
	const EHitTraceWeaponHandIndex HitTraceWeaponHand)
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (!IsValid(AxeCharacterOwner))
	{
		return nullptr;
	}
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
	if (!CombatInterface)
	{
		return nullptr;
	}

	AWeaponEquipmentItemActor* WeaponActor = CombatInterface->
		GetWeaponEquipmentActor_Implementation();
	AWeaponEquipmentItemActor* WeaponActorSecond = CombatInterface->
		GetWeaponEquipmentActorSecondary_Implementation();

	switch (HitTraceWeaponHand)
	{
	case EHitTraceWeaponHandIndex::WeaponHandIndex:
		return WeaponActor;
	case EHitTraceWeaponHandIndex::WeaponSecondaryHandIndex:
		return WeaponActorSecond;
	default:
		break;
	}
	return nullptr;
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
