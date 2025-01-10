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
	SetIsReplicatedByDefault(true);
}

void UComboActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UComboActionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComboAbilityTree(DefaultComboDataAsset);
}

void UComboActionComponent::InitComboAbilityTree(const UComboDataAsset* InComboDataAsset)
{
	if (!InComboDataAsset)
	{
		return;
	}

	ComboAbilityTree = NewObject<UComboTree>();
	UComboTreeNode* RootNode = NewObject<UComboTreeNode>();
	ComboAbilityTree->SetRoot(RootNode);
	LastComboTreeNode = ComboAbilityTree->Root;

	for (const FComboAbilityInfoList& AbilityInfo : InComboDataAsset->ComboAbilityInfo)
	{
		UComboTreeNode* CurrentNode = RootNode;
		for (const FComboAbilityInfo& ComboInfo : AbilityInfo.ComboAbilityTagSequence)
		{
			UComboTreeNode* ChildNode = CurrentNode->FindChildByInputTag(ComboInfo.InputTag);
			if (!ChildNode)
			{
				ChildNode = NewObject<UComboTreeNode>();
				ChildNode->InputTag = ComboInfo.InputTag;

				ChildNode->AbilityTag = ComboInfo.ComboAbilityTag;
				CurrentNode->AddTreeChild(ChildNode);
			}
			CurrentNode = ChildNode;
		}
	}
}

void UComboActionComponent::ResetComboAbilityTree()
{
	ComboAbilityTree = NewObject<UComboTree>();
	UComboTreeNode* RootNode = NewObject<UComboTreeNode>();
	ComboAbilityTree->SetRoot(RootNode);
	LastComboTreeNode = ComboAbilityTree->Root;
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
	if (ComboAbilityTree == nullptr)
	{
		return;
	}
	if (const UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability))
	{
		if (AxeGameplayAbility->GetActivationGroup() != EAxeAbilityActivationGroup::Independent)
		{
			OnAbilityActivated(Ability);
		}
	}
}

void UComboActionComponent::OnNotifyAbilityEnded(UGameplayAbility* Ability)
{
	if (ComboAbilityTree == nullptr)
	{
		return;
	}
	if (const UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability))
	{
		if (AxeGameplayAbility->GetActivationGroup() != EAxeAbilityActivationGroup::Independent)
		{
			OnAbilityEnded(Ability);
		}
	}
}

void UComboActionComponent::OnAbilityInputTagPressed(const FGameplayTag InputTag)
{
	// 统计 输入缓存
	if (bSaveComboInputAbilityTagCache && !bIsInComboWindow && InputTag.IsValid())
	{
		ComboInputAbilityTagCache.Add(InputTag);
	}
}

/**
 * Combo
 */
FGameplayTag UComboActionComponent::GetNextComboAbilityTagByInputTag(
	const FGameplayTag& NextInputAbilityTag)
{
	if (!IsValid(ComboAbilityTree))
	{
		return FGameplayTag::EmptyTag;
	}

	UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChildByInputTag(NextInputAbilityTag);

	if (NextComboTreeNode && IsInComboSwitchWindow())
	{
		// 连招
		return NextComboTreeNode->AbilityTag;
	}
	// 不是连招
	// 从根节点开始查找新的连招
	if (const UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChildByInputTag(NextInputAbilityTag))
	{
		return NewNextComboTreeNode->AbilityTag;
	}
	return FGameplayTag::EmptyTag;
}

bool UComboActionComponent::IsNextComboAbility(const UGameplayAbility* Ability)
{
	const FGameplayTag AbilityTag = AxeAbilitySystemComponent->GetAbilityTag(Ability);
	const UComboTreeNode* ChildNode = LastComboTreeNode->FindChildByAbilityTag(AbilityTag);
	return ChildNode != nullptr;
}

void UComboActionComponent::OnAbilityActivated(UGameplayAbility* Ability)
{
	const FGameplayTag AbilityTag = AxeAbilitySystemComponent->GetAbilityTag(Ability);
	// LastComboTreeNode
	if (UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChildByAbilityTag(AbilityTag))
	{
		// 是连招
		LastComboTreeNode = NextComboTreeNode;
	}
	else
	{
		// 不是连招
		if (UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChildByAbilityTag(AbilityTag))
		{
			LastComboTreeNode = NewNextComboTreeNode;
		}
		else
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
	// ActivatedComboAbility
	if (LastComboTreeNode->AbilityTag.IsValid())
	{
		ActivatedComboAbility = Cast<UAxeGameplayAbility>(Ability);
	}
	else
	{
		ActivatedComboAbility = nullptr;
	}
}

void UComboActionComponent::OnAbilityEnded(UGameplayAbility* Ability)
{
	const FGameplayTag AbilityTag = AxeAbilitySystemComponent->GetAbilityTag(Ability);
	if (AbilityTag.IsValid() && !AbilityTag.MatchesTagExact(LastComboTreeNode->AbilityTag))
	{
		// 连招结束 （不是被连招取消）
		LastComboTreeNode = ComboAbilityTree->Root;
	}

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

void UComboActionComponent::AnsComboSwitchWindowEnd(UAnimNotifyState* NotifyState)
{
	bIsInComboWindow = false;

	if (UAxeAnimNotifyStateBase* AxeAnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(NotifyState))
	{
		// 是否被打断
		bool bIsInterrupted = AxeAnimNotifyStateBase->GetIsInterrupted();

		// 重置连招树
		if (!bIsInterrupted && ActivatedComboAbility)
		{
			FGameplayTag ActivatedTag = AxeAbilitySystemComponent->GetAbilityTag(ActivatedComboAbility);
			if (ActivatedTag.IsValid() && LastComboTreeNode->AbilityTag.MatchesTagExact(ActivatedTag))
			{
				LastComboTreeNode = ComboAbilityTree->Root;
			}
		}
	}
}

void UComboActionComponent::AnsComboInputCacheStart()
{
	ComboInputAbilityTagCache.Reset();
	bSaveComboInputAbilityTagCache = true;
}

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
