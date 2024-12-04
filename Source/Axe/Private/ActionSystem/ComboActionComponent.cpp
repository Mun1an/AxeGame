// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ComboActionComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboDataAsset.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "Net/UnrealNetwork.h"

UComboActionComponent::UComboActionComponent()
{
}

void UComboActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UComboActionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComboAbilityTree();
	LastComboTreeNode = ComboAbilityTree->Root;

	// if (AxeCharacterPlayer->IsLocallyControlled())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("IsLocallyControlled"));
	// }
	// if (AxeCharacterPlayer->HasAuthority())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
	// }
}

void UComboActionComponent::InitComboAbilityTree()
{
	if (!IsValid(ComboDataAsset))
	{
		return;
	}
	ComboAbilityTree = NewObject<UComboTree>();
	UComboTreeNode* RootNode = NewObject<UComboTreeNode>();
	ComboAbilityTree->SetRoot(RootNode);
	for (FComboAbilityInfoList& AbilityInfo : ComboDataAsset->ComboAbilityInfo)
	{
		UComboTreeNode* CurrentNode = RootNode;
		for (const FComboAbilityInfo& ComboInfo : AbilityInfo.ComboAbilityTagSequence)
		{
			UComboTreeNode* ChildNode = CurrentNode->FindChild(ComboInfo.InputTag);
			if (!ChildNode)
			{
				ChildNode = NewObject<UComboTreeNode>();
				ChildNode->IndexTag = ComboInfo.InputTag;
				ChildNode->AbilityClass = ComboInfo.AbilityClass;
				CurrentNode->AddTreeChild(ChildNode);
			}
			CurrentNode = ChildNode;
		}
	}
}

void UComboActionComponent::OnAbilityInitOver()
{
	Super::OnAbilityInitOver();
	if (AxeCharacterPlayer && AxeCharacterPlayer->IsLocallyControlled())
	{
		AxeAbilitySystemComponent->OnNotifyAbilityActivatedDelegate.AddUObject(
			this, &UComboActionComponent::OnNotifyAbilityActivated);
		AxeAbilitySystemComponent->OnNotifyAbilityEndedDelegate.AddUObject(
			this, &UComboActionComponent::OnNotifyAbilityEnded);
		AxeAbilitySystemComponent->OnAbilityInputTagPressedDelegate.AddUObject(
			this, &UComboActionComponent::OnAbilityInputTagPressed);
	}
}

void UComboActionComponent::OnNotifyAbilityActivated(UGameplayAbility* Ability)
{
	OnComboAbilityActivated(Ability);
}

void UComboActionComponent::OnNotifyAbilityEnded(UGameplayAbility* Ability)
{
	OnComboAbilityEnded(Ability);
}

void UComboActionComponent::OnAbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (bSaveComboInputAbilityTagCache && !bIsInComboWindow && InputTag.IsValid())
	{
		ComboInputAbilityTagCache.Add(InputTag);
	}
}

/**
 * Combo
 */
TSubclassOf<UAxeGameplayAbility>* UComboActionComponent::GetComboAbilityByInputTag(
	const FGameplayTag& NextInputAbilityTag)
{
	if (!IsValid(ComboAbilityTree))
	{
		return nullptr;
	}

	if (UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChild(NextInputAbilityTag))
	{
		// 连招
		return &NextComboTreeNode->AbilityClass;
	}
	else
	{
		// 不是连招
		// 从根节点开始查找新的连招
		if (UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChild(NextInputAbilityTag))
		{
			return &NewNextComboTreeNode->AbilityClass;
		}
	}
	return nullptr;
}

bool UComboActionComponent::IsNextComboAbility(const UGameplayAbility* Ability)
{
	UComboTreeNode* ChildNode = LastComboTreeNode->FindChildByAbilityClass(Ability->GetClass());
	return ChildNode != nullptr;
}


void UComboActionComponent::OnComboAbilityActivated(UGameplayAbility* Ability)
{
	// LastComboTreeNode
	if (UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChildByAbilityClass(Ability->GetClass()))
	{
		// 是连招
		LastComboTreeNode = NextComboTreeNode;
	}
	else
	{
		// 不是连招
		if (UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChildByAbilityClass(
			Ability->GetClass()))
		{
			LastComboTreeNode = NewNextComboTreeNode;
		}
		else
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
	// ActivatedComboAbility
	if (LastComboTreeNode->AbilityClass)
	{
		ActivatedComboAbility = Cast<UAxeGameplayAbility>(Ability);
	}
	else
	{
		ActivatedComboAbility = nullptr;
	}
}

void UComboActionComponent::OnComboAbilityEnded(UGameplayAbility* Ability)
{
	bIsInComboWindow = false;
	bSaveComboInputAbilityTagCache = false;
}

UAxeGameplayAbility* UComboActionComponent::GetActivatedComboAbility()
{
	if (ActivatedComboAbility && ActivatedComboAbility->IsActive())
	{
		return ActivatedComboAbility;
	}
	return nullptr;
}


void UComboActionComponent::AnsComboSwitchWindowStart(UAnimNotifyState* NotifyState)
{
	bIsInComboWindow = true;
}

// void UComboActionComponent::AnsComboSwitchWindowTick()
// {
// }

void UComboActionComponent::AnsComboSwitchWindowEnd(UAnimNotifyState* NotifyState)
{
	bIsInComboWindow = false;

	if (UAxeAnimNotifyStateBase* AxeAnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(NotifyState))
	{
		// 是否被打断
		bool bIsInterrupted = AxeAnimNotifyStateBase->GetIsInterrupted();

		// 重置连招树
		if (!bIsInterrupted && ActivatedComboAbility && LastComboTreeNode->AbilityClass == ActivatedComboAbility->
			GetClass())
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
}

void UComboActionComponent::AnsComboInputCacheStart()
{
	ComboInputAbilityTagCache.Reset();
	bSaveComboInputAbilityTagCache = true;
}

// void UComboActionComponent::AnsComboInputCacheTick()
// {
// }

void UComboActionComponent::AnsComboInputCacheEnd()
{
	bSaveComboInputAbilityTagCache = false;

	PressedComboInputInCache();
}

void UComboActionComponent::PressedComboInputInCache()
{
	if (!bIsInComboWindow)
	{
		return;
	}
	if (ComboInputAbilityTagCache.Num() > 0 && AxeAbilitySystemComponent)
	{
		//TODO 也许需要从最后找一个合适的tag
		FGameplayTag InputAbilityTag = ComboInputAbilityTagCache.Pop();
		AxeAbilitySystemComponent->AbilityInputTagPressed(InputAbilityTag);
	}
}
