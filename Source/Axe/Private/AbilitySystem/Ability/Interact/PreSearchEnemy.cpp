// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interact/PreSearchEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "Axe/Axe.h"
#include "Character/AxeCharacterBase.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Interface/EnemyInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/InteractableInterface.h"
#include "UI/Widget/EnemyInfoBarWidgetComponent.h"

UPreSearchEnemy::UPreSearchEnemy(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ActivationPolicy = EAxeAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	OverlapObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UPreSearchEnemy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (IsLocallyControlled())
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		ActorsToIgnore.AddUnique(AxeCharacterOwner);

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(
			ScanTimerHandle, this, &ThisClass::FindTargetAndHandleEnemyInfo, ScanRate, true
		);
	}
}

void UPreSearchEnemy::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                 bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (IsLocallyControlled())
	{
		if (ScanTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ScanTimerHandle);
		}
	}
}

void UPreSearchEnemy::FindTargetAndHandleEnemyInfo()
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	FVector StartLocation = AxeCharacterOwner->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(
		AxeCharacterOwner,
		StartLocation,
		ScanRange,
		OverlapObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);

	//
	for (AActor* OutActor : OutActors)
	{
		if (!OutActor->ActorHasTag(FName("Enemy")))
		{
			continue;
		}
		const IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(OutActor);
		if (!EnemyInterface)
		{
			continue;
		}
		if (UAxeBlueprintFunctionLibrary::IsDead(OutActor))
		{
			continue;
		}
		UEnemyInfoBarWidgetComponent* InfoBarWidgetComponent = EnemyInterface->GetEnemyInfoBarWidgetComponent();
		if (!InfoBarWidgetComponent)
		{
			continue;
		}
		const bool bIsVisible = InfoBarWidgetComponent->IsVisible();
		FVector TargetLocation = OutActor->GetActorLocation();
		float ToTargetDistance = FVector::Dist(StartLocation, TargetLocation);

		if (bIsVisible && ToTargetDistance > InfoBarWidgetComponent->HideBarDistance)
		{
			// Hide
			InfoBarWidgetComponent->SetVisibility(false);
		}
		else if (!bIsVisible && ToTargetDistance < InfoBarWidgetComponent->ShowBarDistance)
		{
			// Show
			InfoBarWidgetComponent->SetVisibility(true);
		}
	}
}
