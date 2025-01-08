// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Instance/EquipmentItemInstance.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Item/Instance/EquipmentItemDefinition.h"
#include "Net/UnrealNetwork.h"

// #if UE_WITH_IRIS
// #include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
// #endif // UE_WITH_IRIS
//
// #include UE_INLINE_GENERATED_CPP_BY_NAME(AxeEquipmentInstance)

class FLifetimeProperty;
class UClass;
class USceneComponent;

UEquipmentItemInstance::UEquipmentItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEquipmentItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedActors);
}

// #if UE_WITH_IRIS
// void UEquipmentInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
// {
// 	using namespace UE::Net;
//
// 	// Build descriptors and allocate PropertyReplicationFragments for this object
// 	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
// }
// #endif // UE_WITH_IRIS

void UEquipmentItemInstance::SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn)
{
	APawn* Owner = GetPawn();
	if (!Owner)
	{
		return;
	}
	USceneComponent* AttachTarget = Owner->GetRootComponent();
	if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(Owner))
	{
		AttachTarget = AxeCharacterPlayer->GetRetargetCharacterMesh();
	}

	for (const FAxeEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
	{
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity,
		                                                          Owner);
		NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform,
		                            SpawnInfo.AttachSocket);

		SpawnedActors.Add(NewActor);
	}
}

void UEquipmentItemInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	SpawnedActors.Empty();
}

void UEquipmentItemInstance::OnEquipped()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnEquipped"));
	K2_OnEquipped();

	const UEquipmentItemDefinition* EquipmentDef = GetDefault<UEquipmentItemDefinition>(GetItemDef());
	SpawnEquipmentActors(EquipmentDef->ActorsToSpawn);

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	const UEquipmentItemDefinition* EquipmentItemDefinition = GetItemDef()->GetDefaultObject<
		UEquipmentItemDefinition>();

	if (AxeCharacterPlayer->HasAuthority() && AxeASC && IsValid(EquipmentItemDefinition->EquipmentEffect))
	{
		FActiveGameplayEffectHandle EffectHandle = AxeASC->ApplyEquipmentEffectToSelf(
			EquipmentItemDefinition->EquipmentEffect, EquipmentItemDefinition->EquipmentAttrInfo,
			EquipmentItemDefinition->ItemTypeTag
		);
		EquipmentEffectHandle = EffectHandle;
	}
}

void UEquipmentItemInstance::OnUnequipped()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnUnequipped"));
	K2_OnUnequipped();

	DestroyEquipmentActors();

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	if (AxeCharacterPlayer->HasAuthority() && AxeASC && EquipmentEffectHandle.IsValid())
	{
		if (EquipmentEffectHandle.IsValid())
		{
			AxeASC->RemoveActiveGameplayEffect(EquipmentEffectHandle);
			EquipmentEffectHandle = FActiveGameplayEffectHandle();
		}
	}
}

UAbilitySystemComponent* UEquipmentItemInstance::GetAbilitySystemComponent()
{
	APawn* Pawn = GetPawn();
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn);
	if (AbilitySystemInterface)
	{
		return AbilitySystemInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}
