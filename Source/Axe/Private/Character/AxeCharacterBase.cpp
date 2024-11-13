// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Axe/Axe.h"
#include "Components/CapsuleComponent.h"
#include "Enum/AxeEnum.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAxeCharacterBase::AAxeCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
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

void AAxeCharacterBase::SetDeath()
{
	SetDeathWithParams(FVector::ZeroVector);
}

void AAxeCharacterBase::SetDeathWithParams(const FVector DeathImpulse)
{
	SetLifeSpan(DeadLifeSpan);
	MulticastDeath(DeathImpulse);
}

void AAxeCharacterBase::InitDefaultAttributes()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	if (DefaultPrimaryAttributesEffect)
	{
		AxeASC->ApplyEffectToSelfByClass(DefaultPrimaryAttributesEffect, 1.f);
	}
	if (DefaultSecondaryAttributesEffect)
	{
		AxeASC->ApplyEffectToSelfByClass(DefaultSecondaryAttributesEffect, 1.f);
	}
}

bool AAxeCharacterBase::GiveStartupAbilities()
{
	if (!HasAuthority())
	{
		return false;
	}
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	if (!AxeASC)
	{
		return false;
	}
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		const bool bGive = AxeASC->GiveAbilityByAbilityAndLevel(Ability, 1);
		if (!bGive)
		{
			return false;
		}
	}
	return true;
}

void AAxeCharacterBase::MulticastDeath_Implementation(const FVector DeathImpulse)
{
	bIsDead = true;

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Impulse
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
}
