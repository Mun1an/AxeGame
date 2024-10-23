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

void UComboActionComponent::OnAbilityInitOver()
{
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterPlayer->AbilitySystemComponent);
	AxeASC->OnNotifyAbilityActivatedDelegate.AddUObject(this, &UComboActionComponent::OnNotifyAbilityActivated);
}

void UComboActionComponent::OnNotifyAbilityActivated(UGameplayAbility* Ability)
{
	if (UComboTreeNode* NextComboTreeNode = LastComboTreeNode->FindChildByRealAbilityClass(Ability->GetClass()))
	{
		// 连招
		LastComboTreeNode = NextComboTreeNode;
	}
	else
	{
		// 未连招
		if (UComboTreeNode* NewNextComboTreeNode = ComboAbilityTree->Root->FindChildByRealAbilityClass(
			Ability->GetClass()))
		{
			LastComboTreeNode = NewNextComboTreeNode;
		}
		else
		{
			LastComboTreeNode = ComboAbilityTree->Root;
		}
	}
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


/**
 * Combo
 */
TSubclassOf<UAxeGameplayAbility>* UComboActionComponent::GetComboAbilityByInputTag(const FGameplayTag& NextInputAbilityTag)
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
	return nullptr;
}


void UComboActionComponent::ComboSwitchWindowStart()
{
	UE_LOG(LogTemp, Warning, TEXT("ComboWindowStart"));
	bIsInComboWindow = true;
}

void UComboActionComponent::ComboSwitchWindowTick()
{
}

void UComboActionComponent::ComboSwitchWindowEnd()
{
	bIsInComboWindow = false;
}
