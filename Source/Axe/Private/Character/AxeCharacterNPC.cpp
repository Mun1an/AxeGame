// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterNPC.h"

#include "Character/AxeCharacterPlayer.h"
#include "Character/InteractableComponent.h"

AAxeCharacterNPC::AAxeCharacterNPC()
{
	bReplicates = true;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
}

TObjectPtr<UInteractableComponent> AAxeCharacterNPC::GetInteractableComponent()
{
	return InteractableComponent;
}

void AAxeCharacterNPC::OnStartBePreInteracting(AAxeCharacterPlayer* InteractPlayer)
{
	if (InteractPlayer->IsLocallyControlled())
	{
		InteractableComponent->SetInteractTextVisibility(true);
	}
}

void AAxeCharacterNPC::OnEndBePreInteracting(AAxeCharacterPlayer* InteractPlayer)
{
	if (InteractPlayer->IsLocallyControlled())
	{
		InteractableComponent->SetInteractTextVisibility(false);
	}
}

void AAxeCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
}
