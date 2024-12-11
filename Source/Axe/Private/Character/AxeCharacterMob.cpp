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

		UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(GetAbilitySystemComponent());
		AxeASC->OnNotifyAbilityActivatedDelegate.AddUObject(this, &ThisClass::OnAbilityActivated);
		AxeASC->OnNotifyAbilityEndedDelegate.AddUObject(this, &ThisClass::OnAbilityEnded);
	}
}

void AAxeCharacterMob::OnAbilityActivated(UGameplayAbility* Ability)
{
	SetIsUsingBlockingAbility();
}

void AAxeCharacterMob::OnAbilityEnded(UGameplayAbility* Ability)
{
	SetIsUsingBlockingAbility();
}

void AAxeCharacterMob::SetIsUsingBlockingAbility()
{
	check(AxeAIController)
	
	const UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	TArray<FGameplayAbilitySpecHandle> SpecHandles;
	AxeASC->GetAbilitySpecHandlesByActivationGroup(
		SpecHandles,
		EAxeAbilityActivationGroup::Exclusive_Blocking
	);
	if (SpecHandles.Num() > 0)
	{
		bIsUsingBlockingAbility = true;
	}
	else
	{
		bIsUsingBlockingAbility = false;
	}
	AxeAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsUsingBlockingAbility"), bIsUsingBlockingAbility);
}
