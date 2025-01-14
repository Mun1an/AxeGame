// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Action/PreItemUseAbility.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Instance/UseItemDefinition.h"
#include "Item/Interface/ItemUseInterface.h"


void UPreItemUseAbility::HandleTryActivateAbilityClientCDO(FGameplayAbilitySpecHandle& AbilitySpecHandle,
                                                           const FGameplayTag AbilityInputTag, AActor* AbilityActor)
{
	Super::HandleTryActivateAbilityClientCDO(AbilitySpecHandle, AbilityInputTag, AbilityActor);

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(AbilityActor);
	UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(ASC->GetOwnerActor(), ASC->GetAvatarActor(), ASC);

	FGameplayEventData Payload;
	Payload.EventTag = AxeGameplayTags.Ability_Action_UseItem;
	Payload.Instigator = AbilityActor;
	Payload.TargetTags = FGameplayTagContainer(AbilityInputTag);

	const bool bSuccess = ASC->TriggerAbilityFromGameplayEvent(
		AbilitySpecHandle,
		&ActorInfo,
		AxeGameplayTags.Ability_Action_UseItem,
		&Payload,
		*ASC
	);

	AbilitySpecHandle = FGameplayAbilitySpecHandle();
}


int32 UPreItemUseAbility::GetInvSlotIndexByUseItemIndex(const int32 UseItemBarIndex)
{
	const UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	TArray<FInventoryEntry> OutEntries;
	InventoryComponent->GetUseItemEntryArray(OutEntries);
	if (OutEntries.IsValidIndex(UseItemBarIndex))
	{
		return OutEntries[UseItemBarIndex].SlotIndex;
	}
	return 0;
}

UInventoryComponent* UPreItemUseAbility::GetOwnerInventoryComponent()
{
	if (!OwnerInventoryComponent)
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		OwnerInventoryComponent = GetInventoryComponent(AxeCharacterOwner);
	}
	return OwnerInventoryComponent;
}

UInventoryComponent* UPreItemUseAbility::GetInventoryComponent(AActor* Actor)
{
	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(Actor))
	{
		return InventoryInterface->GetInventoryComponent();
	}
	return nullptr;
}

void UPreItemUseAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	FGameplayTag AbilityInputTag = TriggerEventData->TargetTags.First();
	if (AbilityInputTag.IsValid())
	{
		bool bActivateExactUseAbility = ActivateExactUseAbility(AbilityInputTag);
		if (bActivateExactUseAbility)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
	}
}


void UPreItemUseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UPreItemUseAbility::ActivateExactUseAbility(FGameplayTag AbilityInputTag)
{
	const int32 UseItemBarIndex = GetUseItemBarIndexByAbilityInputTag(AbilityInputTag);
	if (UseItemBarIndex < 0)
	{
		return false;
	}
	UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	if (!InventoryComponent)
	{
		return false;
	}
	int32 InvSlot = GetInvSlotIndexByUseItemIndex(UseItemBarIndex);
	if (!InventoryComponent->CheckEntryCanItemUse(InvSlot))
	{
		return false;
	}
	UItemInstance* ItemInstance = InventoryComponent->GetItemInstanceByIndex(InvSlot);
	const UItemDefinition* ItemDef = GetDefault<UItemDefinition>(ItemInstance->GetItemDef());
	const UUseItemDefinition* UseItemDefinition = Cast<UUseItemDefinition>(ItemDef);
	if (!UseItemDefinition)
	{
		return false;
	}
	TSubclassOf<UGameplayAbility> NewAbilityClass = UseItemDefinition->GetUseGameplayAbility();
	if (!NewAbilityClass)
	{
		return false;
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEventData Payload;
	Payload.Instigator = GetAxeCharacterOwner();
	Payload.EventMagnitude = UseItemBarIndex;
	
	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(ASC->GetOwnerActor(), ASC->GetAvatarActor(), ASC);
	
	FGameplayAbilitySpec* NewSpec = ASC->FindAbilitySpecFromClass(NewAbilityClass);
	const bool bSuccess = ASC->TriggerAbilityFromGameplayEvent(
		NewSpec->Handle,
		&ActorInfo,
		NewSpec->Ability->AbilityTags.First(),
		&Payload,
		*ASC
	);

	return true;
}

int32 UPreItemUseAbility::GetUseItemBarIndexByAbilityInputTag(const FGameplayTag AbilityInputTag)
{
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	int32 UseItemBarIndex = -1;

	if (!AbilityInputTag.IsValid())
	{
		return UseItemBarIndex;
	}
	if (AbilityInputTag.MatchesTagExact(AxeGameplayTags.InputTag_Action_UseItem_1))
	{
		UseItemBarIndex = 0;
	}
	else if (AbilityInputTag.MatchesTagExact(AxeGameplayTags.InputTag_Action_UseItem_2))
	{
		UseItemBarIndex = 1;
	}
	else if (AbilityInputTag.MatchesTagExact(AxeGameplayTags.InputTag_Action_UseItem_3))
	{
		UseItemBarIndex = 2;
	}
	else if (AbilityInputTag.MatchesTagExact(AxeGameplayTags.InputTag_Action_UseItem_4))
	{
		UseItemBarIndex = 3;
	}

	return UseItemBarIndex;
}
