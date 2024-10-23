// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ComboActionComponent.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboDataAsset.h"
#include "Character/AxeCharacterPlayer.h"

UComboActionComponent::UComboActionComponent()
{
}

void UComboActionComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(Owner);

	if (!AxeCharacterPlayer->IsAbilityInitOver())
	{
		AxeCharacterPlayer->OnAbilityInitOverDelegate.AddUObject(this, &UComboActionComponent::OnAbilityInitOver);
	}
	else
	{
		OnAbilityInitOver();
	}

	InitComboAbilityTree();
	LastComboTreeNode = ComboAbilityTree->Root;
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
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterPlayer->AbilitySystemComponent);
	AxeASC->OnNotifyAbilityActivatedDelegate.AddUObject(this, &UComboActionComponent::OnNotifyAbilityActivated);
}

void UComboActionComponent::OnNotifyAbilityActivated(UGameplayAbility* Ability)
{
	OnComboAbilityActivated(Ability);
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

UAxeAbilitySystemComponent* UComboActionComponent::GetAxeAbilitySystemComponent() const
{
	return Cast<UAxeAbilitySystemComponent>(AxeCharacterPlayer->AbilitySystemComponent);
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

UAxeGameplayAbility* UComboActionComponent::GetActivatedComboAbility()
{
	if (ActivatedComboAbility && ActivatedComboAbility->IsActive())
	{
		return ActivatedComboAbility;
	}
	return nullptr;
}


void UComboActionComponent::ComboSwitchWindowStart()
{
	bIsInComboWindow = true;

	if (UAxeGameplayAbility* ActivatedAbility = GetActivatedComboAbility())
	{
		ActivatedAbility->ChangeActivationGroup(EAxeAbilityActivationGroup::Exclusive_Replaceable);
		ComboSwitchWindowStartAbility = ActivatedAbility;
	}
}

void UComboActionComponent::ComboSwitchWindowTick()
{
}

void UComboActionComponent::ComboSwitchWindowEnd()
{
	bIsInComboWindow = false;
	//
	if (UAxeGameplayAbility* ActivatedAbility = GetActivatedComboAbility())
	{
		ActivatedAbility->ChangeActivationGroup(EAxeAbilityActivationGroup::Exclusive_Blocking);

		// 重置连招树
		if (LastComboTreeNode->AbilityClass == ComboSwitchWindowStartAbility->GetClass())
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
}
