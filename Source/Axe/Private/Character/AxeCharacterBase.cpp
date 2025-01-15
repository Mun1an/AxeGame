// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Axe/Axe.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAxeCharacterBase::AAxeCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}

void AAxeCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

void AAxeCharacterBase::SetDeath()
{
	SetLifeSpan(DeadLifeSpan);

	MulticastDeath(DeathImpulseVector);

	OnDead();
}

void AAxeCharacterBase::OnDead()
{
	OnActorDeadDelegate.Broadcast(this);
}

int32 AAxeCharacterBase::GetCharacterLevel() const
{
	return 1;
}

void AAxeCharacterBase::SetCharacterLevel(int32 NewCharacterLevel)
{
}

void AAxeCharacterBase::InitDefaultAttributes()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	int32 CharacterLevel = GetCharacterLevel();
	if (DefaultPrimaryAttributesEffect)
	{
		AxeASC->ApplyEffectToSelfByClass(DefaultPrimaryAttributesEffect, CharacterLevel);
	}
	if (DefaultSecondaryAttributesEffect)
	{
		AxeASC->ApplyEffectToSelfByClass(DefaultSecondaryAttributesEffect, CharacterLevel);
	}
	if (LevelUpPrimaryAttributesEffect)
	{
		AxeASC->ApplyEffectToSelfByClass(LevelUpPrimaryAttributesEffect, FMath::Max(0, CharacterLevel - 1));
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

void AAxeCharacterBase::MulticastDeath_Implementation(FVector InDeathImpulseVector)
{
	bIsDead = true;

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Impulse
	GetMesh()->AddImpulse(InDeathImpulseVector, NAME_None, true);
}
