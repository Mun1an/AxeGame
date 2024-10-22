// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ComboActionComponent.h"

#include "ActionSystem/ComboDataAsset.h"

UComboActionComponent::UComboActionComponent()
{
}

void UComboActionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComboAbilityTree();
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
			UComboTreeNode* ChildNode = CurrentNode->FindChild(ComboInfo.InputAbilityTag);
			if (!ChildNode)
			{
				ChildNode = NewObject<UComboTreeNode>();
				ChildNode->IndexTag = ComboInfo.InputAbilityTag;
				ChildNode->ValueTag = ComboInfo.RealAbilityTag;
				CurrentNode->AddTreeChild(ChildNode);
			}
			CurrentNode = ChildNode;
		}
	}
}


/**
 * Combo
 */
FGameplayTag UComboActionComponent::GetCombo(FGameplayTag& NextInputAbilityTag)
{
	if (!IsValid(ComboAbilityTree))
	{
		return NextInputAbilityTag;
	}

	if (LastComboTreeNode == nullptr)
	{
		LastComboTreeNode = ComboAbilityTree->Root;
	}

	if (UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChild(NextInputAbilityTag))
	{
		// 连招
		LastComboTreeNode = NextComboTreeNode;
		NextInputAbilityTag = LastComboTreeNode->ValueTag;
	}
	else
	{
		// 未连招
		if (UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChild(NextInputAbilityTag))
		{
			LastComboTreeNode = NewNextComboTreeNode;
		}
		else
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
	return NextInputAbilityTag;
}


void UComboActionComponent::CombatWindowStart()
{
	UE_LOG(LogTemp, Warning, TEXT("CombatWindowStart"));
}

void UComboActionComponent::CombatWindowUpdate()
{
}

void UComboActionComponent::CombatWindowEnd()
{
}
