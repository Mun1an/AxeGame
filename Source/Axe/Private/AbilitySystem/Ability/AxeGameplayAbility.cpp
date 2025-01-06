// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"
#include "AbilitySystem/Tasks/AbilityTask_WaitFacingRot.h"
#include "AbilitySystem/Tasks/AbilityTask_WaitMoveInput.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/AxeMotionWrapAnimNotifyState.h"
#include "Anim/ComboAnimNotifyState.h"
#include "Anim/ComboInputCacheAnimNotifyState.h"
#include "Anim/CustomNameAnimNotifyState.h"
#include "Anim/HitTraceAnimNotifyState.h"
#include "Anim/IgnoreInputAnimNotifyState.h"
#include "Anim/LaunchCharacterNotifyState.h"
#include "Anim/AN/AbilitySkillStageAnimNotify.h"
#include "Anim/AN/CustomNameAnimNotify.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TipsMessage/TipsMessageFunctionLibrary.h"
#include "UI/HUD/AxeHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

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
	return GetAxeCharacterOwner(CurrentActorInfo);
}

AAxeCharacterBase* UAxeGameplayAbility::GetAxeCharacterOwner(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (AActor* LocalAvatarActor = ActorInfo->AvatarActor.Get())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = ActorInfo->OwnerActor.Get();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

AActor* UAxeGameplayAbility::GetOrFindAutoTargetActor()
{
	if (!AutoTargetActor)
	{
		AutoTargetActor = FindOneGoodTargetByMoveInput(
			AutoSearchTargetRadius,
			AutoSearchTargetAngle
		);
	}
	return AutoTargetActor;
}

void UAxeGameplayAbility::SetCurrentAbilitySkillStage(EAbilitySkillStage NewStage)
{
	CurrentAbilitySkillStage = NewStage;
	OnCurrentAbilitySkillStageChanged(NewStage);
}

void UAxeGameplayAbility::OnCurrentAbilitySkillStageChanged(EAbilitySkillStage NewStage)
{
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	// 后摇阶段
	if (NewStage == EAbilitySkillStage::ASS_BackSwing)
	{
		// 后摇阶段时 取消 阻挡标签
		if (IsBlockingOtherAbilities() && bCanReplacedInBackSwing)
		{
			AxeASC->ApplyAbilityBlockAndCancelTags(
				AbilityTags, this, false, BlockAbilitiesWithTag, false, CancelAbilitiesWithTag
			);
		}

		AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetAxeCharacterOwner());
		// 移动取消后摇
		if (AxeCharacterPlayer && bCanCancelByMoveInputInBackSwing)
		{
			UAbilityTask_WaitMoveInput* WaitMoveInputTask = UAbilityTask_WaitMoveInput::CreateWaitMoveInputTask(
				this, AxeCharacterPlayer);
			WaitMoveInputTask->OnMoveInputDelegate.AddUniqueDynamic(
				this, &ThisClass::OnCancelAbilityByMoveInputInBackSwing);
			WaitMoveInputTask->ReadyForActivation();
		}
	}
}

void UAxeGameplayAbility::OnCancelAbilityByMoveInputInBackSwing(FVector LastMoveInput)
{
	if (bCanCancelByMoveInputInBackSwing)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
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

void UAxeGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EAxeAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) ||
				(NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (
				NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

bool UAxeGameplayAbility::CanActivateAbility_ByNowAbilityReplaceCondition(const FGameplayAbilitySpecHandle Handle,
                                                                          const FGameplayAbilityActorInfo* ActorInfo,
                                                                          const FGameplayTagContainer* SourceTags,
                                                                          const FGameplayTagContainer* TargetTags,
                                                                          FGameplayTagContainer* OptionalRelevantTags)
const
{
	/*
	 * 根据上个技能的状态来判断是否可以使用技能
	 * 
	 */

	// 这边的this是 CDO
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (!AxeASC)
	{
		return false;
	}

	// 获取Exclusive_ReplaceableByCondition的使用中的技能
	TArray<FGameplayAbilitySpecHandle> ReplaceableSpecHandles;
	AxeASC->GetAbilitySpecHandlesByActivationGroup(
		ReplaceableSpecHandles,
		EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition
	);

	for (const FGameplayAbilitySpecHandle& SpecHandle : ReplaceableSpecHandles)
	{
		FGameplayAbilitySpec* NowAbilitySpec = AxeASC->FindAbilitySpecFromHandle(SpecHandle);
		if (!NowAbilitySpec)
		{
			continue;
		}
		TArray<UGameplayAbility*> NowGameplayAbilities = NowAbilitySpec->GetAbilityInstances();
		for (UGameplayAbility* NowGameplayAbility : NowGameplayAbilities)
		{
			if (!NowGameplayAbility)
			{
				continue;
			}
			bool bCanReplace = Cast<UAxeGameplayAbility>(NowGameplayAbility)->CanReplaceByNewAbility(this);
			if (bCanReplace)
			{
				return true;
			}
		}
	}

	return false;
}

bool UAxeGameplayAbility::CanReplaceByNewAbility(const UGameplayAbility* NewAbilityCDO) const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(AxeCharacterOwner);
	// 单纯客户端判断了，服务端默认 return true
	if (CharacterPlayer && CharacterPlayer->IsLocallyControlled())
	{
		// Client
		// TODO 继承
		// Check Combo
		UComboActionComponent* ComboActionComponent = CharacterPlayer->GetComboActionComponent();
		bool bIsNextComboAbility = ComboActionComponent->IsNextComboAbility(NewAbilityCDO);
		bool bIsInComboSwitchWindow = ComboActionComponent->IsInComboSwitchWindow();
		if (bIsNextComboAbility && bIsInComboSwitchWindow)
		{
			return true;
		}
		// Check 后摇状态
		const EAbilitySkillStage SkillStage = GetCurrentAbilitySkillStage();
		if (bCanReplacedInBackSwing && SkillStage == EAbilitySkillStage::ASS_BackSwing)
		{
			return true;
		}
		//
		return false;
	}
	// Server
	return true;
}

void UAxeGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

bool UAxeGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags,
                                             FGameplayTagContainer* OptionalRelevantTags) const
{
	// CDO

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	bool bCanActivate = false;

	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	bool bIsActivationGroupBlocked = AxeASC->IsActivationGroupBlocked(ActivationGroup, this);

	bool bCanActivateAbility_ByLastReplaceCondition = CanActivateAbility_ByNowAbilityReplaceCondition(
		Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);


	bCanActivate = !bIsActivationGroupBlocked | bCanActivateAbility_ByLastReplaceCondition;

	return bCanActivate;
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

	SetCurrentAbilitySkillStage(EAbilitySkillStage::ASS_FrontSwing);

	// Client Movement
	if (bUseClientMovement && HasAuthority(&CurrentActivationInfo) && IsForRemoteClient())
	{
		SetIgnoreClientMovementErrorChecksAndCorrection(true);
	}

	// AutoSearchTarget
	if (bNeedAutoSearchTarget && IsLocallyControlled())
	{
		AutoTargetActor = nullptr;
		GetOrFindAutoTargetActor();
	}

	ResetAbilityIgnoreMoveInput();
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

	// Reset Ignore Move Input
	ResetAbilityIgnoreMoveInput();
}

bool UAxeGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);

	UGameplayEffect* CostGE = GetCostGameplayEffect();
	UAbilitySystemComponent* const ASC = ActorInfo->AbilitySystemComponent.Get();

	if (!bResult)
	{
		// 某属性不足时 发送提示
		TArray<FGameplayAttribute> NotApplyAttributes;
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner(ActorInfo);
		UAxeBlueprintFunctionLibrary::CanApplyAttributeModifiers(
			CostGE,
			GetAbilityLevel(Handle, ActorInfo),
			MakeEffectContext(Handle, ActorInfo),
			AxeCharacterOwner,
			NotApplyAttributes
		);
		for (const FGameplayAttribute& NotApplyAttribute : NotApplyAttributes)
		{
			FString String = NotApplyAttribute.GetUProperty()->GetName();
			UTipsMessageFunctionLibrary::SendTipsMessage(
				AxeCharacterOwner, FString::Printf(TEXT("%s 不足"), *String)
			);
		}
	}

	return bResult;
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
	InitMontageNotifyAndNotifyStateClasses(LocalAnimMontage);

	// CustomName
	if (MontageNotifyClassesCache.Contains(UCustomNameAnimNotify::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_CustomName_An = UAbilityTask_MontageNotify::CreateMontageNotifyTask(
			this, LocalAnimMontage,
			UCustomNameAnimNotify::StaticClass()
		);
		AT_CustomName_An->MontageNotifyStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::An_CustomName_NotifyBegin);
		AT_CustomName_An->ReadyForActivation();
	}

	if (MontageNotifyStateClassesCache.Contains(UCustomNameAnimNotifyState::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_CustomName_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UCustomNameAnimNotifyState::StaticClass()
		);
		AT_CustomName_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_CustomName_NotifyBegin);
		AT_CustomName_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_CustomName_NotifyEnd);
		AT_CustomName_Ans->ReadyForActivation();
	}

	// AbilitySkillState
	if (MontageNotifyClassesCache.Contains(UAbilitySkillStageAnimNotify::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_AbilitySkillState_An = UAbilityTask_MontageNotify::CreateMontageNotifyTask(
			this, LocalAnimMontage,
			UAbilitySkillStageAnimNotify::StaticClass()
		);
		AT_AbilitySkillState_An->MontageNotifyStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::An_AbilitySkillState_NotifyBegin);
		AT_AbilitySkillState_An->ReadyForActivation();
	}

	// LaunchCharacter
	if (MontageNotifyStateClassesCache.Contains(ULaunchCharacterNotifyState::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_LaunchCharacter_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			ULaunchCharacterNotifyState::StaticClass()
		);
		AT_LaunchCharacter_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin);
		AT_LaunchCharacter_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd);
		AT_LaunchCharacter_Ans->ReadyForActivation();
	}

	// Ignore Input
	if (MontageNotifyStateClassesCache.Contains(UIgnoreInputAnimNotifyState::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_IgnoreInput_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UIgnoreInputAnimNotifyState::StaticClass()
		);
		AT_IgnoreInput_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyBegin);
		AT_IgnoreInput_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd);
		AT_IgnoreInput_Ans->ReadyForActivation();
	}

	// MotionWrap
	if (MontageNotifyStateClassesCache.Contains(UAxeMotionWrapAnimNotifyState::StaticClass()))
	{
		UAbilityTask_MontageNotify* AT_MotionWrap_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UAxeMotionWrapAnimNotifyState::StaticClass()
		);
		AT_MotionWrap_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_MotionWrap_NotifyBegin);
		AT_MotionWrap_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			this, &UAxeGameplayAbility::Ans_MotionWrap_NotifyEnd);
		AT_MotionWrap_Ans->ReadyForActivation();
	}

	// Combo by IComboAbilityInterface
	if (IComboAbilityInterface* ComboAbility = Cast<IComboAbilityInterface>(this))
	{
		// ComboInputCache
		UAbilityTask_MontageNotify* AT_ComboInputCache_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UComboInputCacheAnimNotifyState::StaticClass()
		);
		AT_ComboInputCache_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_ComboInputCache_NotifyBegin);
		AT_ComboInputCache_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_ComboInputCache_NotifyEnd);
		AT_ComboInputCache_Ans->ReadyForActivation();

		// Combo
		UAbilityTask_MontageNotify* AT_Combo_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
			this, LocalAnimMontage,
			UComboAnimNotifyState::StaticClass()
		);
		AT_Combo_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			ComboAbility, &IComboAbilityInterface::Ans_Combo_NotifyBegin);
		AT_Combo_Ans->MontageNotifyStateEndDelegate.AddDynamic(
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
		AT_HitTrace_Ans->MontageNotifyStateStartDelegate.AddDynamic(
			HitTraceAbility, &IHitTraceAbilityInterface::Ans_HitTrace_NotifyBegin);
		AT_HitTrace_Ans->MontageNotifyStateEndDelegate.AddDynamic(
			HitTraceAbility, &IHitTraceAbilityInterface::Ans_HitTrace_NotifyEnd);
		AT_HitTrace_Ans->ReadyForActivation();
	}
}

void UAxeGameplayAbility::InitMontageNotifyAndNotifyStateClasses(
	UAnimMontage* LocalAnimMontage)
{
	if (MontageNotifyClassesCache.Num() > 0 || MontageNotifyStateClassesCache.Num() > 0)
	{
		return;
	}

	TArray<FAnimNotifyEvent> AnimNotifyEvents = LocalAnimMontage->Notifies;
	for (FAnimNotifyEvent& AnimNotifyEvent : AnimNotifyEvents)
	{
		if (AnimNotifyEvent.Notify.GetClass())
		{
			MontageNotifyClassesCache.AddUnique(AnimNotifyEvent.Notify.GetClass());
		}
		if (AnimNotifyEvent.NotifyStateClass.GetClass())
		{
			MontageNotifyStateClassesCache.AddUnique(AnimNotifyEvent.NotifyStateClass.GetClass());
		}
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

AAxeCharacterBase* UAxeGameplayAbility::FindOneGoodTargetByMoveInput(float SphereRadius, float TraceAngleRange)
{
	AAxeCharacterBase* AxeCharacterBase = GetAxeCharacterOwner();
	AAxeCharacterPlayer* AxeCharacterOwner = Cast<AAxeCharacterPlayer>(AxeCharacterBase);
	if (!AxeCharacterOwner)
	{
		return nullptr;
	}
	TArray<AActor*> OutActors;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(AxeCharacterOwner);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	const FVector OwnerLocation = AxeCharacterOwner->GetActorLocation();
	const FVector LastMovementInputVector = UAxeBlueprintFunctionLibrary::GetAxeLastMovementInputVector(
		AxeCharacterOwner);
	const FVector OwnerForwardVector = AxeCharacterOwner->GetActorForwardVector();
	const FVector TraceDirection = (LastMovementInputVector * 10 + OwnerForwardVector).GetSafeNormal();
	const FVector SpherePos = TraceDirection * 100 + OwnerLocation;
	UKismetSystemLibrary::SphereOverlapActors(
		AxeCharacterOwner,
		SpherePos,
		SphereRadius,
		ObjectTypes,
		nullptr,
		IgnoredActors,
		OutActors
	);
	AAxeCharacterBase* TargetCharacter = nullptr;
	float MinScore = 99999;
	for (AActor* OutActor : OutActors)
	{
		AAxeCharacterBase* OutAxeCharacter = Cast<AAxeCharacterBase>(OutActor);
		if (!OutAxeCharacter)
		{
			continue;
		}
		FVector LocalActorLocation = OutAxeCharacter->GetActorLocation();
		FVector ToTargetDir = (LocalActorLocation - OwnerLocation).GetSafeNormal();
		// 判断是否在指定方向的前方
		const float DotProduct = FVector::DotProduct(ToTargetDir, TraceDirection);
		const float Angle = FMath::Acos(DotProduct) * 180.f / PI;
		if (Angle > TraceAngleRange)
		{
			continue;
		}
		// 结合距离和角度计算分数
		FVector PlaneNormal = FVector::CrossProduct(TraceDirection, FVector::UpVector);
		const float PointPlaneDistance = FMath::Abs(
			FVector::PointPlaneDist(LocalActorLocation, OwnerLocation, PlaneNormal));
		const float ToCharacterDistance = (LocalActorLocation - OwnerLocation).Size();
		const float TargetScore = PointPlaneDistance * 2 + ToCharacterDistance;
		if (TargetScore < MinScore)
		{
			TargetCharacter = OutAxeCharacter;
			MinScore = TargetScore;
		}
	}
	return TargetCharacter;
}

void UAxeGameplayAbility::SetAbilityIgnoreMoveInput(bool bIsIgnore)
{
	const AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AbilityIgnoreMoveInputCount += bIsIgnore ? 1 : -1;
	AbilityIgnoreMoveInputCount = FMath::Max(AbilityIgnoreMoveInputCount, 0);

	if (AController* Controller = AxeCharacterOwner->GetController())
	{
		Controller->SetIgnoreMoveInput(bIsIgnore);
	}
}

void UAxeGameplayAbility::ResetAbilityIgnoreMoveInput()
{
	if (AbilityIgnoreMoveInputCount > 0)
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		if (AController* Controller = AxeCharacterOwner->GetController())
		{
			for (int i = 0; i < AbilityIgnoreMoveInputCount; ++i)
			{
				Controller->SetIgnoreMoveInput(false);
			}
		}
		AbilityIgnoreMoveInputCount = 0;
	}
}

void UAxeGameplayAbility::An_AbilitySkillState_NotifyBegin(UAnimNotify* AnimNotify)
{
	// UE_LOG(LogTemp, Warning, TEXT("An_BackSwing_NotifyBegin"));
	const UAbilitySkillStageAnimNotify* Notify = Cast<UAbilitySkillStageAnimNotify>(AnimNotify);
	if (Notify)
	{
		SetCurrentAbilitySkillStage(Notify->GetAbilitySkillStage());
	}
}

void UAxeGameplayAbility::An_CustomName_NotifyBegin(UAnimNotify* AnimNotify)
{
	const FName CustomName = Cast<UCustomNameAnimNotify>(AnimNotify)->CustomName;
	BP_OnCustomNameNotifyBegin(CustomName);
}

void UAxeGameplayAbility::Ans_CustomName_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	const FName CustomName = Cast<UCustomNameAnimNotifyState>(AnimNotifyState)->CustomName;
	BP_OnCustomNameAnsBegin(CustomName);
}

void UAxeGameplayAbility::Ans_CustomName_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	const FName CustomName = Cast<UCustomNameAnimNotifyState>(AnimNotifyState)->CustomName;
	BP_OnCustomNameAnsEnd(CustomName);
}

void UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	ULaunchCharacterNotifyState* Launch_ANS = Cast<ULaunchCharacterNotifyState>(AnimNotifyState);
	const FVector MotionDirection = ActionCombatComponent->GetLaunchDirectionByEnum(
		AxeCharacterOwner, Launch_ANS->LaunchDirection
	);
	float LaunchSpeed = Launch_ANS->LaunchSpeed;
	float NotifyStateDuration = Launch_ANS->GetNotifyStateDuration();

	// 根据 目标位置 调整降低计算位移
	if (Launch_ANS->bDynamicChangedByTargetDistance)
	{
		const AActor* TargetActor = GetOrFindAutoTargetActor();
		if (TargetActor)
		{
			const FVector TargetPos = TargetActor->GetActorLocation();
			const FVector SourcePos = AxeCharacterOwner->GetActorLocation();
			float ToTargetDist = (TargetPos - SourcePos).Size();
			// 暂时减去碰撞盒距离
			ToTargetDist = FMath::Max(ToTargetDist - 100, 1.f);

			float OldNearlyDistance = LaunchSpeed * NotifyStateDuration;
			if (OldNearlyDistance > ToTargetDist)
			{
				float MotionMulti = ToTargetDist / OldNearlyDistance;
				LaunchSpeed = LaunchSpeed * MotionMulti;
				NotifyStateDuration = NotifyStateDuration * MotionMulti;
			}
		}
	}

	UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		NAME_None,
		MotionDirection,
		LaunchSpeed,
		NotifyStateDuration,
		false,
		nullptr,
		ERootMotionFinishVelocityMode::ClampVelocity,
		FVector::ZeroVector,
		100,
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
		SetAbilityIgnoreMoveInput(true);
	}
}

void UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		SetAbilityIgnoreMoveInput(false);
	}
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	// 朝附近的目标吸附
	if (!IsLocallyControlled())
	{
		return;
	}
	// Set Ignore Move Input
	SetAbilityIgnoreMoveInput(true);
	UAxeMotionWrapAnimNotifyState* AxeANS = Cast<UAxeMotionWrapAnimNotifyState>(AnimNotifyState);
	if (!AxeANS)
	{
		return;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AActor* AttachTarget = GetOrFindAutoTargetActor();
	if (AttachTarget == nullptr)
	{
		return;
	}
	const FVector TargetLocation = AttachTarget->GetActorLocation();
	const FVector OwnerLocation = AxeCharacterOwner->GetActorLocation();
	const FVector ToTargetDir = (TargetLocation - OwnerLocation);
	const float ToTargetDist = ToTargetDir.Size();
	// MoveTo
	if (AxeANS->IsMoveToTarget())
	{
		const FVector LastMovementInputVector = UAxeBlueprintFunctionLibrary::GetAxeLastMovementInputVector(
			Cast<AAxeCharacterPlayer>(AxeCharacterOwner)
		);
		if (AxeANS->IsMoveToTargetOnMoveInput() && LastMovementInputVector.IsNearlyZero(0.01))
		{
			// 如果没输入方向就不移动了
		}
		else
		{
			// ApplyRootMotionConstantForce
			float MotionDuration = AxeANS->GetNotifyStateDuration();
			float MotionStrength = (ToTargetDist / MotionDuration) * AxeANS->GetTraceSpeedCoefficient();

			UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
				this,
				NAME_None,
				ToTargetDir,
				MotionStrength,
				MotionDuration,
				false,
				nullptr,
				ERootMotionFinishVelocityMode::ClampVelocity,
				FVector::ZeroVector,
				100,
				true
			);
		}
	}
	// SetActorRotation
	if (AxeANS->IsFacingTarget())
	{
		const FRotator ToRot = ToTargetDir.Rotation().GetNormalized();
		UAbilityTask_WaitFacingRot* AbilityTask_WaitFacingRot = UAbilityTask_WaitFacingRot::CreateWaitFacingRotTask(
			this, AxeCharacterOwner, ToRot, 50.0f
		);
		AbilityTask_WaitFacingRot->ReadyForActivation();
	}
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		SetAbilityIgnoreMoveInput(false);
	}
}
