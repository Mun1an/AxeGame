// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AxeActionComponentBase.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterPlayer.h"

UAxeActionComponentBase::UAxeActionComponentBase()
{
}

void UAxeActionComponentBase::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(Owner);

	if (!AxeCharacterPlayer->IsAbilityInitOver())
	{
		AxeCharacterPlayer->OnAbilityInitOverDelegate.AddUObject(this, &UAxeActionComponentBase::OnAbilityInitOver);
	}
	else
	{
		OnAbilityInitOver();
	}
}

void UAxeActionComponentBase::OnAbilityInitOver()
{
	AxeAbilitySystemComponent = Cast<UAxeAbilitySystemComponent>(AxeCharacterPlayer->GetAbilitySystemComponent());
}
