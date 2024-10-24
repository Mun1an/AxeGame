// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterBase.h"

#include "Enum/AxeEnum.h"

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

void AAxeCharacterBase::SetCustomLaunchCharacter(float LaunchSpeed, ELaunchCharacterDirection LaunchDirection,
                                                 bool bXYOverride, bool bZOverride)
{
	FVector LaunchVelocity = FVector::ZeroVector;

	switch (LaunchDirection)
	{
	case ELaunchCharacterDirection::Lc_Forward:
		LaunchVelocity = GetActorForwardVector();
		break;
	case ELaunchCharacterDirection::Lc_Backward:
		LaunchVelocity = GetActorForwardVector() * -1;
		break;
	case ELaunchCharacterDirection::Lc_Left:
		LaunchVelocity = GetActorRightVector() * -1;
		break;
	case ELaunchCharacterDirection::Lc_Right:
		LaunchVelocity = GetActorRightVector();
		break;
	case ELaunchCharacterDirection::Lc_Up:
		LaunchVelocity = GetActorUpVector();
		break;
	case ELaunchCharacterDirection::Lc_Down:
		LaunchVelocity = GetActorUpVector() * -1;
		break;
	default:
		break;
	}

	LaunchVelocity *= LaunchSpeed;
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}
