// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterMob.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

AAxeCharacterMob::AAxeCharacterMob()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAxeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAxeAttributeSet>("AttributeSet");
}

void AAxeCharacterMob::BeginPlay()
{
	Super::BeginPlay();

	//
	if (HasAuthority())
	{
		InitDefaultAttributes();
	}
}
