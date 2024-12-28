// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ModularCharacterComponent.h"

#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UModularCharacterComponent::UModularCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UModularCharacterComponent::SetModularMesh()
{
	const AAxeCharacterPlayer* Owner = GetAxeCharacterPlayerOwner();
	USkeletalMeshComponent* RetargetCharacterMesh = Owner->GetRetargetCharacterMesh();
	for (const TTuple<EAxeModularCharacterSM, USkeletalMeshComponent*>& Pair : Owner->GetModularSkeletalMeshComponentMap()
	)
	{
		if (Pair.Value == nullptr)
		{
			continue;
		}
		Pair.Value->SetLeaderPoseComponent(RetargetCharacterMesh);
	}
}

// Called when the game starts
void UModularCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
}

AAxeCharacterPlayer* UModularCharacterComponent::GetAxeCharacterPlayerOwner()
{
	if (AxeCharacterPlayerOwner == nullptr)
	{
		AActor* Actor = GetOwner();
		AxeCharacterPlayerOwner = Cast<AAxeCharacterPlayer>(Actor);
	}
	return AxeCharacterPlayerOwner;
}
