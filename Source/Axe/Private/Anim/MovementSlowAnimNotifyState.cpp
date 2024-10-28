// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/MovementSlowAnimNotifyState.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"

void UMovementSlowAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent(MeshComp);
	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);
	if (AxeCharacterPlayer == nullptr)
	{
		return;
	}
	UActionCombatComponent* ActionCombatComponent = AxeCharacterPlayer->GetActionCombatComponent();
	if (AxeASC == nullptr || ActionCombatComponent == nullptr)
	{
		return;
	}
	const UAnimMontage* SourceAnimMontage = Cast<UAnimMontage>(EventReference.GetSourceObject());
	UAxeGameplayAbility* GameplayAbility = AxeASC->GetActivationAbilityByAbilityMontage(SourceAnimMontage);
	if (GameplayAbility)
	{
		const float EffectMagnitude = GameplayAbility->GetAbilityUsingMovementSlowEffectMagnitude();
		ActionCombatComponent->ApplyMovementSlowEffectInAbilityUse(EffectMagnitude, TotalDuration);
	}
}

void UMovementSlowAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);
	if (AxeCharacterPlayer == nullptr)
	{
		return;
	}
	UActionCombatComponent* ActionCombatComponent = AxeCharacterPlayer->GetActionCombatComponent();
	if (ActionCombatComponent)
	{
		ActionCombatComponent->RemoveMovementSlowEffectInAbilityUse();
	}
}
