// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/Interaction/ComboAbilityInterface.h"
#include "AbilitySystem/Interaction/HitTraceAbilityInterface.h"
#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"
#include "AbilitySystem/Tasks/AbilityTask_WaitFacingRot.h"
#include "AbilitySystem/Tasks/AbilityTask_WaitLastMoveInput.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/AxeMotionWrapAnimNotifyState.h"
#include "Anim/ComboAnimNotifyState.h"
#include "Anim/ComboInputCacheAnimNotifyState.h"
#include "Anim/HitTraceAnimNotifyState.h"
#include "Anim/IgnoreInputAnimNotifyState.h"
#include "Anim/LaunchCharacterNotifyState.h"
#include "Anim/AN/BackSwingAnimNotify.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAxeGameplayAbility::SetAbilitySkillStage(EAbilitySkillStage NewStage)
{
	AbilitySkillStage = NewStage;
	OnAbilitySkillStageChanged(NewStage);
}

void UAxeGameplayAbility::OnAbilitySkillStageChanged(EAbilitySkillStage NewStage)
{
	if (NewStage == EAbilitySkillStage::ASS_BackSwing)
	{
		UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
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

bool UAxeGameplayAbility::CanActivateAbility_ByLastReplaceCondition(const FGameplayAbilitySpecHandle Handle,
                                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                                    const FGameplayTagContainer* SourceTags,
                                                                    const FGameplayTagContainer* TargetTags,
                                                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	// 这边的this是 CDO
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (!AxeASC)
	{
		return false;
	}

	TArray<FGameplayAbilitySpecHandle> SpecHandles;
	AxeASC->GetAbilitySpecHandlesByActivationGroup(
		SpecHandles,
		EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition
	);

	for (const FGameplayAbilitySpecHandle& SpecHandle : SpecHandles)
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = AxeASC->FindAbilitySpecFromHandle(SpecHandle);
		if (!GameplayAbilitySpec)
		{
			continue;
		}
		TArray<UGameplayAbility*> GameplayAbilities = GameplayAbilitySpec->GetAbilityInstances();
		for (UGameplayAbility* LastGameplayAbility : GameplayAbilities)
		{
			if (!LastGameplayAbility)
			{
				continue;
			}
			bool EachResult = CanActivateAbility_ByLastReplaceCondition_EachProxy(LastGameplayAbility, ActorInfo);
			if (EachResult)
			{
				return true;
			}
		}
	}

	return false;
}

bool UAxeGameplayAbility::CanActivateAbility_ByLastReplaceCondition_EachProxy(
	UGameplayAbility* LastAbility, const FGameplayAbilityActorInfo* ActorInfo) const
{
	// 这边的this是 CDO ,,, LastAbility 不是CDO

	AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(GetAxeCharacterOwner(ActorInfo));
	if (!CharacterPlayer)
	{
		return false;
	}

	// 单纯客户端判断了，服务端默认 return true
	if (CharacterPlayer->IsLocallyControlled())
	{
		// Client
		UComboActionComponent* ComboActionComponent = CharacterPlayer->GetComboActionComponent();
		bool bIsNextComboAbility = ComboActionComponent->IsNextComboAbility(this);
		bool bIsInComboSwitchWindow = ComboActionComponent->IsInComboSwitchWindow();
		if (bIsNextComboAbility && bIsInComboSwitchWindow)
		{
			return true;
		}

		const UAxeGameplayAbility* LastAxeAbility = Cast<UAxeGameplayAbility>(LastAbility);
		const EAbilitySkillStage SkillStage = LastAxeAbility->GetAbilitySkillStage();
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

	bool bCanActivateAbility_ByLastReplaceCondition = CanActivateAbility_ByLastReplaceCondition(
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
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	SetAbilitySkillStage(EAbilitySkillStage::Ass_FrontSwing);

	// Client Movement
	if (bUseClientMovement && HasAuthority(&CurrentActivationInfo) && IsForRemoteClient())
	{
		SetIgnoreClientMovementErrorChecksAndCorrection(true);
	}

	// AutoSearchTarget
	if (bNeedAutoSearchTarget && IsLocallyControlled())
	{
		GetOrFindAutoTargetActor();
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

	// LaunchCharacter
	UAbilityTask_MontageNotify* AT_BackSwing_An = UAbilityTask_MontageNotify::CreateMontageNotifyTask(
		this, LocalAnimMontage,
		UBackSwingAnimNotify::StaticClass()
	);
	AT_BackSwing_An->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::An_BackSwing_NotifyBegin);
	AT_BackSwing_An->ReadyForActivation();

	// LaunchCharacter
	UAbilityTask_MontageNotify* AT_LaunchCharacter_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		ULaunchCharacterNotifyState::StaticClass()
	);
	AT_LaunchCharacter_Ans->MontageNotifyStateStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin);
	AT_LaunchCharacter_Ans->MontageNotifyStateEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd);
	AT_LaunchCharacter_Ans->ReadyForActivation();

	// Ignore Input
	UAbilityTask_MontageNotify* AT_IgnoreInput_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		UIgnoreInputAnimNotifyState::StaticClass()
	);
	AT_IgnoreInput_Ans->MontageNotifyStateStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyBegin);
	AT_IgnoreInput_Ans->MontageNotifyStateEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd);
	AT_IgnoreInput_Ans->ReadyForActivation();

	// MotionWrap
	UAbilityTask_MontageNotify* AT_MotionWrap_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, LocalAnimMontage,
		UAxeMotionWrapAnimNotifyState::StaticClass()
	);
	AT_MotionWrap_Ans->MontageNotifyStateStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MotionWrap_NotifyBegin);
	AT_MotionWrap_Ans->MontageNotifyStateEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MotionWrap_NotifyEnd);
	AT_MotionWrap_Ans->ReadyForActivation();

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
		float PointPlaneDistance = FVector::PointPlaneDist(LocalActorLocation, OwnerLocation, PlaneNormal);
		float ToCharacterDistance = (LocalActorLocation - OwnerLocation).Size();
		if (PointPlaneDistance * 2 + ToCharacterDistance < MinScore)
		{
			TargetCharacter = OutAxeCharacter;
		}
	}
	return TargetCharacter;
}

void UAxeGameplayAbility::An_BackSwing_NotifyBegin(UAnimNotify* AnimNotify)
{
	UE_LOG(LogTemp, Warning, TEXT("An_BackSwing_NotifyBegin"));

	SetAbilitySkillStage(EAbilitySkillStage::ASS_BackSwing);
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
		true,
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
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		if (AController* Controller = AxeCharacterOwner->GetController())
		{
			Controller->SetIgnoreMoveInput(true);
		}
	}
}

void UAxeGameplayAbility::Ans_IgnoreInput_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		if (AController* Controller = AxeCharacterOwner->GetController())
		{
			Controller->SetIgnoreMoveInput(false);
		}
	}
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	// 朝附近的目标吸附
	if (!IsLocallyControlled())
	{
		return;
	}
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

	FVector ToTargetDir = (AttachTarget->GetActorLocation() - AxeCharacterOwner->GetActorLocation());
	float ToTargetDist = ToTargetDir.Size();

	// SetIgnoreMoveInput
	if (AController* Controller = AxeCharacterOwner->GetController())
	{
		Controller->SetIgnoreMoveInput(true);
		bHasSetIgnoreMoveInputByMotionWrap = true;
	}

	// ApplyRootMotionConstantForce
	float MotionDuration = AxeANS->GetNotifyStateDuration();
	float MotionStrength = (ToTargetDist / MotionDuration) * AxeANS->GetTraceSpeedCoefficient();

	UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		NAME_None,
		ToTargetDir,
		MotionStrength,
		MotionDuration,
		true,
		nullptr,
		ERootMotionFinishVelocityMode::ClampVelocity,
		FVector::ZeroVector,
		100,
		true
	);

	// SetActorRotation
	const FRotator ToRot = ToTargetDir.Rotation().GetNormalized();
	UAbilityTask_WaitFacingRot* AbilityTask_WaitFacingRot = UAbilityTask_WaitFacingRot::CreateWaitFacingRotTask(
		this, AxeCharacterOwner, ToRot, 50.0f
	);
	AbilityTask_WaitFacingRot->ReadyForActivation();
}

void UAxeGameplayAbility::Ans_MotionWrap_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsLocallyControlled())
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		if (AController* Controller = AxeCharacterOwner->GetController())
		{
			if (bHasSetIgnoreMoveInputByMotionWrap)
			{
				Controller->SetIgnoreMoveInput(false);
				bHasSetIgnoreMoveInputByMotionWrap = false;
			}
		}
	}
}
