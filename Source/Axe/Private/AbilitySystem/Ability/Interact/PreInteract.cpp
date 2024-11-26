// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interact/PreInteract.h"

#include "AbilitySystemComponent.h"
#include "Axe/Axe.h"
#include "Character/AxeCharacterBase.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/InteractableInterface.h"

UPreInteract::UPreInteract(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ActivationPolicy = EAxeAbilityActivationPolicy::OnSpawn;

	OverlapObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_Interact));
}

void UPreInteract::TriggerInteraction()
{
	if (CurrentTarget == nullptr)
	{
		return;
	}
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = AxeGameplayTags.Ability_Interaction_Activate;
	Payload.Instigator = AxeCharacterOwner;
	Payload.Target = CurrentTarget;

	FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(
		CurrentInteractionOption.InteractionAbilityToGrant);

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
	ActorsToIgnore.AddUnique(AxeCharacterOwner);

	AActor* GoodTarget = GetGoodTarget();

	CurrentTarget = GoodTarget;
	if (Cast<IInteractableInterface>(CurrentTarget))
	{
		Cast<IInteractableInterface>(CurrentTarget)->GetInteractionOptions(CurrentInteractionOption);
		CurrentInteractionOption.InteractableTarget = CurrentTarget;
	}
	else
	{
		CurrentInteractionOption = FInteractionOption::Empty;
	}

	if (CurrentTarget)
	{
		DrawDebugSphere(
			GetWorld(), CurrentTarget->GetActorLocation(), 50, 10, FColor::Red, false, 0.2
		);
	}
}

AActor* UPreInteract::GetGoodTarget()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
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
		if (Angle > ScanAngleRange)
		{
			continue;
		}
		// 结合距离和角度计算分数
		FVector PlaneNormal = FVector::CrossProduct(TraceDirection, FVector::UpVector);
		const float PointPlaneDistance = FMath::Abs(
			FVector::PointPlaneDist(TargetLocation, OwnerLocation, PlaneNormal));
		const float ToCharacterDistance = (TargetLocation - OwnerLocation).Size();
		const float TargetScore = PointPlaneDistance * 2 + ToCharacterDistance;
		if (TargetScore < MinScore)
		{
			TargetActor = OutActor;
			MinScore = TargetScore;
		}
	}
	return TargetActor;
}
