// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/MovementSlowAnimNotifyState.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"

void UMovementSlowAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent(MeshComp);
	UComboActionComponent* PlayerComboActionComponent = GetPlayerComboActionComponent(MeshComp);
	if (AxeASC == nullptr || PlayerComboActionComponent == nullptr)
	{
		return;
	}
	const UAnimMontage* SourceAnimMontage = Cast<UAnimMontage>(EventReference.GetSourceObject());
	UAxeGameplayAbility* GameplayAbility = AxeASC->GetActivationAbilityByAbilityMontage(SourceAnimMontage);
	if (GameplayAbility)
	{
		const float EffectMagnitude = GameplayAbility->GetAbilityUsingMovementSlowEffectMagnitude();
		PlayerComboActionComponent->ApplyMovementSlowEffectInAbilityUse(EffectMagnitude, TotalDuration);
	}
}

void UMovementSlowAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UComboActionComponent* PlayerComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		PlayerComboActionComponent->RemoveMovementSlowEffectInAbilityUse();
	}
}
