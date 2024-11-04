// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ActionCombatComponent.h"

#include "GameplayEffectTypes.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Enum/AxeEnum.h"
#include "GameFramework/Character.h"

UActionCombatComponent::UActionCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FActiveGameplayEffectHandle UActionCombatComponent::ApplyMovementSlowEffectInAbilityUse(const float Level,
	const float Duration)
{
	check(AxeAbilitySystemComponent);

	FGameplayEffectContextHandle ContextHandle = AxeAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(GetOwner());
	const FGameplayEffectSpecHandle SpecHandle = AxeAbilitySystemComponent->MakeOutgoingSpec(
		MovementSlowEffectClass, Level, ContextHandle
	);
	SpecHandle.Data->SetDuration(Duration, true);
	FActiveGameplayEffectHandle EffectHandle = AxeAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), AxeAbilitySystemComponent
	);

	return EffectHandle;
}

bool UActionCombatComponent::RemoveMovementSlowEffectInAbilityUse()
{
	AxeAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(
		MovementSlowEffectClass, AxeAbilitySystemComponent, 1
	);
	return true;
}

void UActionCombatComponent::SetCustomLaunchCharacter(float LaunchSpeed, ELaunchCharacterDirection LaunchDirection,
                                                      bool bXYOverride, bool bZOverride)

{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	FVector LaunchVelocity = GetLaunchDirectionByEnum(Character, LaunchDirection);

	LaunchVelocity *= LaunchSpeed;
	Character->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

FVector UActionCombatComponent::GetLaunchDirectionByEnum(AActor* Actor, ELaunchCharacterDirection LaunchDirectionEnum)
{
	FVector LaunchDir = FVector::ZeroVector;
	switch (LaunchDirectionEnum)
	{
	case ELaunchCharacterDirection::Lc_Forward:
		LaunchDir = Actor->GetActorForwardVector();
		break;
	case ELaunchCharacterDirection::Lc_Backward:
		LaunchDir = Actor->GetActorForwardVector() * -1;
		break;
	case ELaunchCharacterDirection::Lc_Left:
		LaunchDir = Actor->GetActorRightVector() * -1;
		break;
	case ELaunchCharacterDirection::Lc_Right:
		LaunchDir = Actor->GetActorRightVector();
		break;
	case ELaunchCharacterDirection::Lc_Up:
		LaunchDir = Actor->GetActorUpVector();
		break;
	case ELaunchCharacterDirection::Lc_Down:
		LaunchDir = Actor->GetActorUpVector() * -1;
		break;
	default:
		break;
	}
	return LaunchDir;
}

void UActionCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UActionCombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
