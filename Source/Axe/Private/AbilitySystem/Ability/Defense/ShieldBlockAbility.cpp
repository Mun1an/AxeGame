// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Defense/ShieldBlockAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied_Self.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystem/Tasks/AbilityTask_OnDamageExecution.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "Character/AxeCharacterBase.h"


void UShieldBlockAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}


void UShieldBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	bIsBlocking = false;
	bIsPrepareParry = false;
	bIsParrying = false;
	
	// AbilityTask_WaitInputRelease
	UAbilityTask_WaitInputRelease* AbilityTask_WaitInputRelease =
		UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	AbilityTask_WaitInputRelease->OnRelease.AddDynamic(this, &UShieldBlockAbility::OnInputReleased);
	AbilityTask_WaitInputRelease->ReadyForActivation();
	//
	AActor* OwningActor = GetAxeCharacterOwner();
	UAbilityTask_OnDamageExecution* AbilityTask_OnDamageExecution =
		UAbilityTask_OnDamageExecution::CreateOnBeDamagedExecutionTask(this, OwningActor);
	AbilityTask_OnDamageExecution->OnBeDamagedExecutionDelegate.AddDynamic(this, &UShieldBlockAbility::OnBeDamagedCal);
	AbilityTask_OnDamageExecution->ReadyForActivation();
	//
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	FGameplayTagRequirements SourceTagRequirements = FGameplayTagRequirements();
	SourceTagRequirements.RequireTags.AddTag(AxeGameplayTags.Effect_Asset_Damage);
	UAbilityTask_WaitGameplayEffectApplied_Self* AbilityTask_WaitGameplayEffectApplied_Self =
		UAbilityTask_WaitGameplayEffectApplied_Self::WaitGameplayEffectAppliedToSelf(
			this,
			FGameplayTargetDataFilterHandle(),
			SourceTagRequirements,
			FGameplayTagRequirements(),
			false,
			nullptr,
			false
		);
	AbilityTask_WaitGameplayEffectApplied_Self->OnApplied.AddDynamic(this, &UShieldBlockAbility::OnEffectApplied);
	AbilityTask_WaitGameplayEffectApplied_Self->ReadyForActivation();

	//
	UAbilityTask_WaitDelay* AbilityTask_WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, PrePareParryTime);
	AbilityTask_WaitDelay->OnFinish.AddDynamic(this, &UShieldBlockAbility::OnPrePareParryFinished);
	AbilityTask_WaitDelay->ReadyForActivation();

	//
	SetIsBlocking(true);
	SetIsPrepareParry(true);
}

void UShieldBlockAbility::OnPrePareParryFinished()
{
	SetIsPrepareParry(false);
}

void UShieldBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//
	SetIsBlocking(false);
	SetIsPrepareParry(false);
}


void UShieldBlockAbility::OnInputReleased(float TimeHeld)
{
	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


bool UShieldBlockAbility::CanApplyEffectAttributeModifiers(FGameplayEffectSpecHandle& EffectSpecHandle)
{
	return true;
}

void UShieldBlockAbility::OnBeDamagedCal(UDamageCalInfo* DamageCalInfo)
{
	// TODO

	AActor* DamageSourceActor = DamageCalInfo->DamageSourceActor;

	bool bIsInBlockAngle = IsInBlockAngle(DamageSourceActor);
	if (!bIsInBlockAngle)
	{
		return;
	}

	//
	DamageCalInfo->Damage = 0;
	DamageCalInfo->bIsBlocked = true;

	// ShieldParry
	if (bIsPrepareParry)
	{
		bIsParrying = true;
		DamageCalInfo->bIsParried = true;
		TransformToShieldParry(DamageSourceActor);
	}
}

void UShieldBlockAbility::JumpBlockMontageToHitSection(FName SectionName)
{
	ClientJumpBlockMontageToHitSection(SectionName);
}

void UShieldBlockAbility::ClientJumpBlockMontageToHitSection_Implementation(FName SectionName)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return ;
	}
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return ;
	}
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (ASC->GetAnimatingAbility() == this && ASC->GetCurrentMontage() == AbilityMontage)
	{
		// Unbind delegates so they don't get called as well
		FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(AbilityMontage);
		MontageInstance->JumpToSectionName(SectionName);
	}
}


bool UShieldBlockAbility::ApplyShieldBlockDamageCostEffect(float CostValue)
{
	if (!ShieldBlockDamageCostEffectCls || !HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		return false;
	}
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(
		CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		ShieldBlockDamageCostEffectCls, 1.f
	);
	if (!EffectSpecHandle.IsValid())
	{
		return false;
	}
	// Check
	EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
		FAxeGameplayTags::Get().Effect_Magnitude_Stamina, CostValue
	);
	// Apply
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToOwner(
		CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		EffectSpecHandle
	);
	return ActiveGameplayEffectHandle.IsValid();
}

bool UShieldBlockAbility::TransformToShieldParry(AActor* Source)
{
	bool bResult = false;
	if (!ShieldParryAbilityClass)
	{
		return false;
	}
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	// self
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	// TryActivateAbilityByClass
	bResult |= AxeASC->TryActivateAbilityByClass(ShieldParryAbilityClass);
	// Source HitReact
	AAxeCharacterBase* SourceAxeCharacterBase = Cast<AAxeCharacterBase>(Source);
	if (SourceAxeCharacterBase)
	{
		UAbilitySystemComponent* SourceASC = SourceAxeCharacterBase->GetAbilitySystemComponent();
		UAxeAbilitySystemComponent* SourceAxeASC = Cast<UAxeAbilitySystemComponent>(SourceASC);
		SourceAxeASC->TryActivateHitReactAbility(AxeGameplayTags.Ability_HitReact_Light, FHitResult());
	}
	return bResult;
}

bool UShieldBlockAbility::TransformToShieldStagger(AActor* Source)
{
	bool bResult = false;
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	// self
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	if (ShieldStaggerAbilityClass)
	{
		bResult |= AxeASC->TryActivateAbilityByClass(ShieldStaggerAbilityClass);
	}
	return bResult;
}

bool UShieldBlockAbility::IsInBlockAngle(AActor* AttackSource) const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (!AxeCharacterOwner)
	{
		return false;
	}
	FVector OwnerForward = AxeCharacterOwner->GetActorForwardVector();
	FVector OwnerToAttackSource = AttackSource->GetActorLocation() - AxeCharacterOwner->GetActorLocation();
	OwnerForward.Z = 0;
	OwnerToAttackSource.Z = 0;
	OwnerForward.Normalize();
	OwnerToAttackSource.Normalize();
	float Dot = FVector::DotProduct(OwnerForward, OwnerToAttackSource);
	float Angle = FMath::Acos(Dot) * 180 / PI;
	return Angle <= BlockAngle;
}

void UShieldBlockAbility::SetIsBlocking(bool NewIsBlocking)
{
	bIsBlocking = NewIsBlocking;

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterOwner->AbilitySystemComponent);
	if (NewIsBlocking)
	{
		AxeASC->AddLooseGameplayTag(AxeGameplayTags.State_Blocking);
	}
	else
	{
		AxeASC->RemoveLooseGameplayTag(AxeGameplayTags.State_Blocking);
	}
}

void UShieldBlockAbility::SetIsPrepareParry(bool NewIsPrepareParry)
{
	bIsPrepareParry = NewIsPrepareParry;
	bIsParrying = false;

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterOwner->AbilitySystemComponent);
	if (NewIsPrepareParry)
	{
		AxeASC->AddLooseGameplayTag(AxeGameplayTags.State_PrepareParry);
	}
	else
	{
		AxeASC->RemoveLooseGameplayTag(AxeGameplayTags.State_PrepareParry);
	}
}

void UShieldBlockAbility::OnEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
                                          FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayEffectSpec* GameplayEffectSpec = SpecHandle.Data.Get();

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	const UAttributeSet* AttributeSet = AxeASC->GetAttributeSet(UAxeAttributeSet::StaticClass());
	const UAxeAttributeSet* AxeAttributeSet = Cast<UAxeAttributeSet>(AttributeSet);

	TArray<FGameplayEffectModifiedAttribute> ModifiedAttributes = GameplayEffectSpec->ModifiedAttributes;
	for (FGameplayEffectModifiedAttribute& ModifiedAttribute : ModifiedAttributes)
	{
		if (ModifiedAttribute.Attribute == AxeAttributeSet->GetIncomingDamageAttribute())
		{
			OnIncomingDamageEffectApplied(Source, SpecHandle, ActiveHandle);
			break;
		}
	}
}

void UShieldBlockAbility::OnIncomingDamageEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
                                                        FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayEffectContextHandle EffectContextHandle = SpecHandle.Data.Get()->GetContext();
	FAxeGameplayEffectContext AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		EffectContextHandle);
	const float PreDamageValue = AxeEffectContext.GetPreDamageValue();
	UObject* SourceObject = AxeEffectContext.GetSourceObject();
	AActor* DamageSourceActor = Cast<AActor>(SourceObject);

	if (PreDamageValue > 0 && !bIsParrying)
	{
		// JumpBlockMontageToHitSection
		JumpBlockMontageToHitSection();

		// Cost
		float CostValue = FMath::Abs(PreDamageValue) * -1;
		ApplyShieldBlockDamageCostEffect(CostValue);

		// ShieldStagger
		float NowStamina = Cast<UAxeAttributeSet>(GetAxeCharacterOwner()->GetAttributeSet())->GetStamina();
		if (NowStamina <= 0)
		{
			TransformToShieldStagger(DamageSourceActor);
		}
	}
}
