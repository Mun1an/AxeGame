// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "ActionSystem/ComboActionComponent.h"
#include "ActionSystem/ComboDataAsset.h"
#include "Character/AxeCharacterPlayer.h"

UAxeAbilitySystemComponent::UAxeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UAxeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	TArray<FGameplayAbilitySpec> GameplayAbilitySpecList = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				TryActivateAbilityAndCheck(AbilitySpec.Handle);
			}
		}
	}
}

void UAxeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// TArray<FGameplayAbilitySpec> GameplayAbilitySpecList = GetActivatableAbilities();
	// for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	// {
	// 	if (!AbilitySpec.IsActive() && AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag))
	// 	{
	// 		TryActivateAbilitiesByTag(AbilitySpec.Handle);
	// 	}
	// }
}

void UAxeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	//
}

void UAxeAbilitySystemComponent::TryActivateAbilityAndCheck(FGameplayAbilitySpecHandle AbilityToActivate,
                                                            bool bAllowRemoteActivation)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityToActivate);

	FGameplayTag InputTag = Cast<UAxeGameplayAbility>(Spec->Ability)->InputTag;
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetAvatarActor());
	// combo
	if (InputTag.IsValid() && IsValid(AxeCharacterPlayer))
	{
		UComboActionComponent* ComboActionComponent = AxeCharacterPlayer->GetComboActionComponent();
		TSubclassOf<UAxeGameplayAbility>* AbilityClass = ComboActionComponent->GetComboAbilityByInputTag(InputTag);
		if (AbilityClass != nullptr)
		{
			FGameplayAbilitySpec* NewAbilitySpec = FindAbilitySpecFromClass(*AbilityClass);
			if (NewAbilitySpec != nullptr)
			{
				AbilityToActivate = NewAbilitySpec->Handle;
			}
		}
	}
	TryActivateAbility(AbilityToActivate, bAllowRemoteActivation);
}

void UAxeAbilitySystemComponent::GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability,
                                                              const int32 AbilityLevel)
{
	const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	GiveAbility(AbilitySpec);
}

bool UAxeAbilitySystemComponent::IsActivationGroupBlocked(EAxeAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	const TArray<UGameplayAbility*>* Abilities = nullptr;
	switch (Group)
	{
	case EAxeAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EAxeAbilityActivationGroup::Exclusive_Replaceable:
	case EAxeAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		Abilities = ActivationGroupMap.Find(EAxeAbilityActivationGroup::Exclusive_Blocking);
		if (Abilities != nullptr)
		{
			bBlocked = Abilities->Num() > 0;
		}
		break;
	default:
		break;
	}

	return bBlocked;
}

void UAxeAbilitySystemComponent::AddAbilityToActivationGroup(EAxeAbilityActivationGroup Group,
                                                             UAxeGameplayAbility* AxeAbility)
{
	ActivationGroupMap.FindOrAdd(Group).AddUnique(AxeAbility);

	TArray<UGameplayAbility*> Abilities = TArray<UGameplayAbility*>();
	switch (Group)
	{
	case EAxeAbilityActivationGroup::Independent:
		break;
	case EAxeAbilityActivationGroup::Exclusive_Replaceable:
	case EAxeAbilityActivationGroup::Exclusive_Blocking:
		Abilities = ActivationGroupMap.FindOrAdd(EAxeAbilityActivationGroup::Exclusive_Replaceable);
		for (UGameplayAbility* Ability : Abilities)
		{
			if (Ability == AxeAbility)
			{
				continue;
			}
			CancelAbility(Ability);
		}
		break;
	default:
		break;
	}
}

void UAxeAbilitySystemComponent::RemoveAbilityFromActivationGroup(EAxeAbilityActivationGroup Group,
                                                                  UAxeGameplayAbility* AxeAbility)
{
	ActivationGroupMap.FindOrAdd(Group).Remove(AxeAbility);
}

void UAxeAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	// Add InputTag
	if (UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(AbilitySpec.Ability))
	{
		if (AxeGameplayAbility->InputTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AxeGameplayAbility->InputTag);
		}
	}
}

void UAxeAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
                                                        UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability);
	AddAbilityToActivationGroup(AxeGameplayAbility->GetActivationGroup(), AxeGameplayAbility);

	OnNotifyAbilityActivatedDelegate.Broadcast(Ability);
}

void UAxeAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
                                                    bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability);
	RemoveAbilityFromActivationGroup(AxeGameplayAbility->GetActivationGroup(), AxeGameplayAbility);
}
