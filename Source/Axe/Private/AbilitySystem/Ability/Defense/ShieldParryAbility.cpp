// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Defense/ShieldParryAbility.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "GameplayTag/AxeGameplayTags.h"

void UShieldParryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && HasAuthority(&CurrentActivationInfo))
	{
		UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
		TArray<FAxeAttributeTagAndValue> AttributeTagAndValues;
		AxeASC->ApplyAddAttrDurationEffectByTag(
			GetAxeCharacterOwner(),
			FAxeGameplayTags::Get().Attributes_Secondary_PhysicalResistance,
			1.0f,
			1.0f
		);
	}
}
