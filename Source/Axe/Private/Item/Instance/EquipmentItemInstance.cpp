// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Instance/EquipmentItemInstance.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "Item/ItemFunctionLibrary.h"
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
	DOREPLIFETIME(ThisClass, EquipmentItemInstanceInfo);
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
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());

	const UEquipmentItemDefinition* EquipmentDef = GetDefault<UEquipmentItemDefinition>(GetItemDef());
	if (AxeCharacterPlayer && AxeCharacterPlayer->HasAuthority())
	{
		SpawnEquipmentActors(EquipmentDef->ActorsToSpawn);
	}

	UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	const UEquipmentItemDefinition* EquipmentItemDefinition = GetItemDef()->GetDefaultObject<
		UEquipmentItemDefinition>();

	if (AxeCharacterPlayer->HasAuthority() && AxeASC && IsValid(EquipmentItemDefinition->EquipmentEffect))
	{
		FActiveGameplayEffectHandle EffectHandle = AxeASC->ApplyEquipmentEffectToSelf(
			EquipmentItemDefinition->EquipmentEffect, EquipmentItemInstanceInfo,
			EquipmentItemDefinition->ItemTypeTag
		);
		EquipmentEffectHandle = EffectHandle;
	}
}

void UEquipmentItemInstance::OnUnequipped()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnUnequipped"));
	K2_OnUnequipped();
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());

	if (AxeCharacterPlayer && AxeCharacterPlayer->HasAuthority())
	{
		DestroyEquipmentActors();
	}

	UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent();
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

FEquipmentItemInstanceInfo& UEquipmentItemInstance::GetEquipmentItemInstanceInfo()
{
	return EquipmentItemInstanceInfo;
}

void UEquipmentItemInstance::SetEquipmentItemInstanceInfo(const FEquipmentItemInstanceInfo& InEquipmentItemInstanceInfo)
{
	EquipmentItemInstanceInfo = InEquipmentItemInstanceInfo;
}

void UEquipmentItemInstance::InitEquipmentItemInstanceInfo(int32 EquipmentLevel, EEquipmentRarity EquipmentRarity)
{
	const UEquipmentItemDefinition* EquipmentDef = GetDefault<UEquipmentItemDefinition>(GetItemDef());
	EquipmentItemInstanceInfo.EquipmentLevel = EquipmentLevel;
	EquipmentItemInstanceInfo.EquipmentRarity = EquipmentRarity;
	EquipmentItemInstanceInfo.CalDamage(EquipmentDef->EquipmentDefaultLevelInfo.EquipmentDamage);
	EquipmentItemInstanceInfo.CalArmor(EquipmentDef->EquipmentDefaultLevelInfo.EquipmentArmor);
	EquipmentItemInstanceInfo.CalMaxHealth(EquipmentDef->EquipmentDefaultLevelInfo.EquipmentMaxHealth);
}

void UEquipmentItemInstance::OnItemInstanceCreated()
{
	Super::OnItemInstanceCreated();
	InitEquipmentItemInstanceInfo(1);
}

FString UEquipmentItemInstance::GetItemDescription()
{
	return FString::Printf(
		TEXT(
			"等级：%d \n"
			"伤害：%d "
		),
		EquipmentItemInstanceInfo.EquipmentLevel,
		FMath::RoundToInt(EquipmentItemInstanceInfo.EquipmentDamage
		)
	);
}
