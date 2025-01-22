// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interact/PreInteract.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Axe/Axe.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "Character/InteractableComponent.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/InteractableInterface.h"

UPreInteract::UPreInteract(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ActivationPolicy = EAxeAbilityActivationPolicy::OnSpawn;

	OverlapObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECC_Pawn),
	};
}

void UPreInteract::TriggerInteraction()
{
	if (CurrentTarget == nullptr)
	{
		return;
	}
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = AxeGameplayTags.Ability_Interaction_Activate;
	Payload.Instigator = AxeCharacterOwner;
	Payload.Target = CurrentTarget;

	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(
		CurrentInteractionOption.InteractionAbilityToGrant);
	if (AbilitySpec == nullptr)
	{
		return;
	}
	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(CurrentActorInfo->OwnerActor.Get(), CurrentActorInfo->AvatarActor.Get(), ASC);
	const bool bSuccess = ASC->TriggerAbilityFromGameplayEvent(
		AbilitySpec->Handle,
		&ActorInfo,
		AxeGameplayTags.Ability_Interaction_Activate,
		&Payload,
		*ASC
	);
}


void UPreInteract::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWorld* World = GetWorld();

	ActorsToIgnore.AddUnique(GetAxeCharacterOwner());

	World->GetTimerManager().SetTimer(
		ScanTimerHandle, this, &ThisClass::FindTargetInteractable, ScanRate, true);
}

void UPreInteract::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                              bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ScanTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ScanTimerHandle);
	}
}

void UPreInteract::FindTargetInteractable()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(AxeCharacterOwner);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();


	AActor* GoodTarget = GetGoodTarget();

	CurrentTarget = GoodTarget;
	IInteractableInterface* CurrentInteractableInterface = Cast<IInteractableInterface>(CurrentTarget);
	if (CurrentInteractableInterface)
	{
		UInteractableComponent* InteractableComponent = CurrentInteractableInterface->GetInteractableComponent();
		CurrentInteractionOption = InteractableComponent->GetInteractionOptions();
		CurrentInteractionOption.InteractableTarget = CurrentTarget;
	}
	else
	{
		CurrentInteractionOption = FInteractionOption::Empty;
	}
	// 添加一下玩家新交互技能
	if (CurrentInteractionOption.InteractionAbilityToGrant && HasAuthority(&CurrentActivationInfo))
	{
		// Fixme 可优化
		const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(
			CurrentInteractionOption.InteractionAbilityToGrant);
		if (AbilitySpec == nullptr)
		{
			const FGameplayAbilitySpec NewAbilitySpec = FGameplayAbilitySpec(
				CurrentInteractionOption.InteractionAbilityToGrant, 1);
			ASC->GiveAbility(NewAbilitySpec);
		}
	}

	if (LastCurrentTarget != CurrentTarget)
	{
		if (CurrentInteractableInterface)
		{
			CurrentInteractableInterface->OnStartBePreInteracting(AxeCharacterPlayer);
		}
		if (LastCurrentTarget)
		{
			if (IInteractableInterface* LastInteractableInterface = Cast<IInteractableInterface>(LastCurrentTarget))
			{
				LastInteractableInterface->OnEndBePreInteracting(AxeCharacterPlayer);
			}
		}
		LastCurrentTarget = CurrentTarget;
		OnTargetInteractableChange();
	}
}

AActor* UPreInteract::GetGoodTarget()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner == nullptr)
	{
		return nullptr;
	}
	FVector OwnerLocation = AxeCharacterOwner->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(
		AxeCharacterOwner,
		OwnerLocation,
		ScanRange,
		OverlapObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);
	float MinScore = 99999;
	AActor* TargetActor = nullptr;
	for (AActor* OutActor : OutActors)
	{
		TScriptInterface<IInteractableInterface> InteractableActor(OutActor);
		if (!InteractableActor)
		{
			continue;
		}
		FVector TargetLocation = OutActor->GetActorLocation();
		FVector ToTargetDir = (TargetLocation - OwnerLocation).GetSafeNormal();
		FVector TraceDirection = AxeCharacterOwner->GetActorForwardVector();
		// 判断是否在指定方向的前方
		const float DotProduct = FVector::DotProduct(ToTargetDir, TraceDirection);
		const float Angle = FMath::Acos(DotProduct) * 180.f / PI;
		float TargetScore = 0;
		if (Angle > ScanAngleRange)
		{
			TargetScore += Angle;
		}
		// 结合距离和角度计算分数
		FVector PlaneNormal = FVector::CrossProduct(TraceDirection, FVector::UpVector);
		const float PointPlaneDistance = FMath::Abs(
			FVector::PointPlaneDist(TargetLocation, OwnerLocation, PlaneNormal));
		const float ToCharacterDistance = (TargetLocation - OwnerLocation).Size();
		TargetScore += PointPlaneDistance * 2 + ToCharacterDistance;
		if (TargetScore < MinScore)
		{
			TargetActor = OutActor;
			MinScore = TargetScore;
		}
	}
	return TargetActor;
}
