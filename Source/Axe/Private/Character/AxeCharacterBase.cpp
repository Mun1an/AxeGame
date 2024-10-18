// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterBase.h"

// Sets default values
AAxeCharacterBase::AAxeCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAxeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAxeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAxeCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AAxeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

