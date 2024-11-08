// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "AbilitySystem/Tasks/AbilityTask_ApplyEffect.h"
#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/ComboAnimNotifyState.h"
#include "Anim/ComboInputCacheAnimNotifyState.h"
#include "Anim/HitTraceAnimNotifyState.h"
#include "Anim/IgnoreInputAnimNotifyState.h"
#include "Anim/LaunchCharacterNotifyState.h"
#include "Anim/MovementSlowAnimNotifyState.h"
#include "Character/AxeCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interaction/CombatInterface.h"

#define ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(FunctionName, ReturnValue)																				\
{																																						\
	if (!ensure(IsInstantiated()))																														\
	{																																					\
		ABILITY_LOG(Error, TEXT("%s: " #FunctionName " cannot be called on a non-instanced ability. Check the instancing policy."), *GetPathName());	\
		return ReturnValue;																																\
	}																																					\
}

AAxeCharacterBase* UAxeGameplayAbility::GetAxeCharacterOwner() const
{
	if (AActor* LocalAvatarActor = CurrentActorInfo->AvatarActor.Get())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = CurrentActorInfo->OwnerActor.Get();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

bool UAxeGameplayAbility::CanChangeActivationGroup(EAxeAbilityActivationGroup NewGroup) const
{
	if (!IsInstantiated() || !IsActive())
	{
		return false;
	}

	if (ActivationGroup == NewGroup)
	{
		return true;
	}

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	check(AxeASC);

	if ((ActivationGroup != EAxeAbilityActivationGroup::Exclusive_Blocking) &&
		AxeASC->IsActivationGroupBlocked(NewGroup, this)
	)
	{
		return false;
	}

	if ((NewGroup == EAxeAbilityActivationGroup::Exclusive_Replaceable) && !CanBeCanceled())
	{
		return false;
	}
	if ((NewGroup == EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition) && !CanBeCanceled())
	{
		return false;
	}

	return true;
}

bool UAxeGameplayAbility::ChangeActivationGroup(EAxeAbilityActivationGroup NewGroup)
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(ChangeActivationGroup, false);

	if (!CanChangeActivationGroup(NewGroup))
	{
		return false;
	}

	if (ActivationGroup != NewGroup)
	{
		UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();

		AxeASC->RemoveAbilityFromActivationGroup(ActivationGroup, this);
		AxeASC->AddAbilityToActivationGroup(NewGroup, this);
		ActivationGroup = NewGroup;
	}

	return true;
}

bool UAxeGameplayAbility::CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const
{
	return true;
}

bool UAxeGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags,
                                             FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (AxeASC->IsActivationGroupBlocked(ActivationGroup, this))
	{
		return false;
	}

	return true;
}

void UAxeGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UAxeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Client Movement
	if (bUseClientMovement && HasAuthority(&CurrentActivationInfo) && IsForRemoteClient())
	{
		SetIgnoreClientMovementErrorChecksAndCorrection(true);
	}
}

void UAxeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Client Movement
	if (bUseClientMovement && HasAuthority(&CurrentActivationInfo) && IsForRemoteClient())
	{
		SetIgnoreClientMovementErrorChecksAndCorrection(false);
	}
}


//
UAxeAbilitySystemComponent* UAxeGameplayAbility::GetAxeAbilitySystemComponentFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		return Cast<UAxeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	}
	return nullptr;
}

UActionCombatComponent* UAxeGameplayAbility::GetActionCombatComponent() const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (CombatInterface)
		{
			return CombatInterface->GetActionCombatComponent();
		}
	}
	return nullptr;
}

UComboActionComponent* UAxeGameplayAbility::GetComboActionComponent() const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (CombatInterface)
		{
			return CombatInterface->GetComboActionComponent();
		}
	}
	return nullptr;
}

void UAxeGameplayAbility::SetActiveMontagePauseFrame(float LowRate, float Duration)
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UAnimInstance* AnimInstance = AxeCharacterOwner->GetMesh()->GetAnimInstance();
	FAnimMontageInstance* ActiveMontageInstance = AnimInstance->GetActiveInstanceForMontage(AbilityMontage);
	if (ActiveMontageInstance)
	{
		if (ActiveMontagePauseFrameTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ActiveMontagePauseFrameTimerHandle);
		}
		ActiveMontagePauseFrameInstance = ActiveMontageInstance;
		const float PlayRate = LowRate * ActiveMontagePauseFrameRateMagnitude;
		const float PlayDuration = Duration * ActiveMontagePauseFrameDurationMagnitude;
		ActiveMontageInstance->SetPlayRate(PlayRate);
		GetWorld()->GetTimerManager().SetTimer(
			ActiveMontagePauseFrameTimerHandle,
			this,
			&UAxeGameplayAbility::ActiveMontagePauseFrameEnd,
			PlayDuration,
			false
		);
	}
}

void UAxeGameplayAbility::ActiveMontagePauseFrameEnd()
{
	if (ActiveMontagePauseFrameInstance && ActiveMontagePauseFrameInstance->bPlaying)
	{
		ActiveMontagePauseFrameInstance->SetPlayRate(1.0);
	}
}

void UAxeGameplayAbility::ShakeCamera()
{
	if (!CameraShakeClass)
	{
		return;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		APlayerController* PlayerController = AxeCharacterOwner->GetLocalViewingPlayerController();
		if (PlayerController)
		{
			PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, 1.0f);
		}
	}
}

void UAxeGameplayAbility::AddMontageNotifyStateTask(UAnimMontage* LocalAnimMontage)
{
	//AddTask

	//UAbilityTask_MontageNotify

	// MovementSlow
	UAbilityTask_MontageNotify* AT_MovementSlow_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		UMovementSlowAnimNotifyState::StaticClass()
	);
	AT_MovementSlow_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MovementSlow_NotifyBegin);
	AT_MovementSlow_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MovementSlow_NotifyEnd);
	AT_MovementSlow_Ans->ReadyForActivation();

	// LaunchCharacter
	UAbilityTask_MontageNotify* AT_LaunchCharacter_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		ULaunchCharacterNotifyState::StaticClass()
	);
	AT_LaunchCharacter_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin);
	AT_LaunchCharacter_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd);
	AT_LaunchCharacter_Ans->ReadyForActivation();

	// Ignore Input
	UAbilityTask_MontageNotify* AT_IgnoreInput_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		UIgnoreInputAnimNotifyState::StaticClass()
	);
	AT_IgnoreInput_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyBegin);
	AT_IgnoreInput_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd);
	AT_IgnoreInput_Ans->ReadyForActivation();

	// Combo by IComboAbilityInterface
	if (IComboAbilityInterface* ComboAbility = Cast<IComboAbilityInterface>(this))
	{
		// ComboInputCache
		UAbilityTask_MontageNotify* AT_ComboInputCache_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UComboInputCacheAnimNotifyState::StaticClass()
		);
		AT_ComboInputCache_Ans->MontageNotifyStartDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_ComboInputCache_NotifyBegin);
		AT_ComboInputCache_Ans->MontageNotifyEndDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_ComboInputCache_NotifyEnd);
		AT_ComboInputCache_Ans->ReadyForActivation();

		// Combo
		UAbilityTask_MontageNotify* AT_Combo_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UComboAnimNotifyState::StaticClass()
		);
		AT_Combo_Ans->MontageNotifyStartDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_Combo_NotifyBegin);
		AT_Combo_Ans->MontageNotifyEndDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_Combo_NotifyEnd);
		AT_Combo_Ans->ReadyForActivation();
	}

	// HitTrace by IHitTraceAbilityInterface
	if (IHitTraceAbilityInterface* HitTraceAbility = Cast<IHitTraceAbilityInterface>(this))
	{
		UAbilityTask_MontageNotify* AT_HitTrace_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UHitTraceAnimNotifyState::StaticClass()
		);
		AT_HitTrace_Ans->MontageNotifyStartDelegate.AddDynamic(HitTraceAbility,
		                                                       &IHitTraceAbilityInterface::Ans_HitTrace_NotifyBegin);
		AT_HitTrace_Ans->MontageNotifyEndDelegate.AddDynamic(HitTraceAbility,
		                                                     &IHitTraceAbilityInterface::Ans_HitTrace_NotifyEnd);
		AT_HitTrace_Ans->ReadyForActivation();
	}
}

void UAxeGameplayAbility::SetIgnoreClientMovementErrorChecksAndCorrection(bool bIsIgnore)
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AxeCharacterOwner->GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = bIsIgnore;
	AxeCharacterOwner->GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = bIsIgnore;
}

//
void UAxeGameplayAbility::Ans_MovementSlow_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	// UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	// UAxeAnimNotifyStateBase* AxeAnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(AnimNotifyState);
	// AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	// UAbilityTask_ApplyEffect* AbilityTask_ApplyEffect = UAbilityTask_ApplyEffect::CreateApplyEffectTask(
	// 	this, AxeCharacterOwner, AxeCharacterOwner,
	// 	ActionCombatComponent->GetMovementSlowEffectClass(),
	// 	AbilityUsingMovementSlowEffectMagnitude,
	// 	AxeAnimNotifyStateBase->GetNotifyStateDuration()
	// );
	// AbilityTask_ApplyEffect->ReadyForActivation();
}


void UAxeGameplayAbility::Ans_MovementSlow_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	// UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	// if (ActionCombatComponent)
	// {
	// 	ActionCombatComponent->RemoveMovementSlowEffectInAbilityUse();
	// }
}


void UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	ULaunchCharacterNotifyState* LaunchCharacterNotifyState = Cast<ULaunchCharacterNotifyState>(AnimNotifyState);
	const FVector MotionDirection = ActionCombatComponent->GetLaunchDirectionByEnum(
		GetAxeCharacterOwner(), LaunchCharacterNotifyState->LaunchDirection
	);
	float LaunchSpeed = LaunchCharacterNotifyState->LaunchSpeed;
	float NotifyStateDuration = LaunchCharacterNotifyState->GetNotifyStateDuration();

	UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		NAME_None,
		MotionDirection,
		LaunchSpeed,
		NotifyStateDuration,
		true,
		nullptr,
		ERootMotionFinishVelocityMode::ClampVelocity,
		FVector::ZeroVector,
		1000,
		true
	);
}


void UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
}

void UAxeGameplayAbility::Ans_IgnoreInput_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		AController* Controller = AxeCharacterOwner->GetController();
		Controller->SetIgnoreMoveInput(true);
	}
}

void UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		AController* Controller = AxeCharacterOwner->GetController();
		Controller->SetIgnoreMoveInput(false);
	}
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
}
