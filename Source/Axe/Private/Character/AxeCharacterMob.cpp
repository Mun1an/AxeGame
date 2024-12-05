// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterMob.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AI/AxeAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAxeCharacterMob::AAxeCharacterMob()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAxeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAxeAttributeSet>("AttributeSet");
	
}

void AAxeCharacterMob::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (HasAuthority())
	{
		check(BehaviorTree)
		
		AxeAIController = Cast<AAxeAIController>(NewController);
		AxeAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AxeAIController->RunBehaviorTree(BehaviorTree);
	}
}

void AAxeCharacterMob::BeginPlay()
{
	Super::BeginPlay();

	//
	if (HasAuthority())
	{
		InitDefaultAttributes();
		GiveStartupAbilities();
	}
}
