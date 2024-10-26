// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Enum/AxeEnum.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AAxeCharacterBase::SetWalkSpeed(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(NewMaxWalkSpeed, MinWalkSpeed, MaxWalkSpeed);
}

void AAxeCharacterBase::SetRotationRateZ(float NewRotationRateZ)
{
	GetCharacterMovement()->RotationRate.Yaw = FMath::Clamp(NewRotationRateZ, MinRotationRateZ, MaxRotationRateZ);
}

void AAxeCharacterBase::SetRotationRateByWalkSpeed()
{
	float NewRotationRateZ = FMath::GetMappedRangeValueClamped(
		FVector2D(MinWalkSpeed, MaxWalkSpeed),
		FVector2D(MinRotationRateZ, MaxRotationRateZ),
		GetCharacterMovement()->MaxWalkSpeed
	);
	SetRotationRateZ(NewRotationRateZ);
}

void AAxeCharacterBase::InitDefaultAttributes()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	AxeASC->ApplyEffectToSelfByClass(DefaultPrimaryAttributesEffect, 1.f);
	AxeASC->ApplyEffectToSelfByClass(DefaultSecondaryAttributesEffect, 1.f);
}
