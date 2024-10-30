// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "ActionSystem/ComboActionComponent.h"
#include "ActionSystem/ComboDataAsset.h"
#include "Character/AxeCharacterPlayer.h"

UAxeAbilitySystemComponent::UAxeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

AAxeCharacterBase* UAxeAbilitySystemComponent::GetAxeCharacterOwner() const
{
	if (AActor* LocalAvatarActor = GetAvatarActor())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = GetOwner();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

void UAxeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	TArray<FGameplayAbilitySpec>& GameplayAbilitySpecList = GetActivatableAbilities();
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
	OnAbilityInputTagPressedDelegate.Broadcast(InputTag);
}

void UAxeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
}

void UAxeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	//
}

void UAxeAbilitySystemComponent::TryActivateAbilityAndCheck(FGameplayAbilitySpecHandle AbilitySpecHandle,
                                                            bool bAllowRemoteActivation)
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	const FGameplayTag InputTag = Cast<UAxeGameplayAbility>(AbilitySpec->Ability)->InputTag;
	const AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetAxeCharacterOwner());

	// Combo
	if (InputTag.IsValid() && IsValid(AxeCharacterPlayer))
	{
		UComboActionComponent* ComboActionComponent = AxeCharacterPlayer->GetComboActionComponent();
		TSubclassOf<UAxeGameplayAbility>* AbilityClass = ComboActionComponent->GetComboAbilityByInputTag(InputTag);
		if (AbilityClass != nullptr)
		{
			FGameplayAbilitySpec* NewAbilitySpec = FindAbilitySpecFromClass(*AbilityClass);
			if (NewAbilitySpec != nullptr)
			{
				AbilitySpecHandle = NewAbilitySpec->Handle;
			}
		}
	}

	TryActivateAbility(AbilitySpecHandle, bAllowRemoteActivation);
	//
	// GEngine->AddOnScreenDebugMessage(
	// 	-1, 3.0f, FColor::Blue,
	// 	FString::Printf(
	// 		TEXT("Ability TryActivate: %s"), *FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability->GetName()
	// 	)
	// );
}

void UAxeAbilitySystemComponent::GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability,
                                                              const int32 AbilityLevel)
{
	const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	GiveAbility(AbilitySpec);
}

bool UAxeAbilitySystemComponent::IsActivationGroupBlocked(EAxeAbilityActivationGroup Group,
                                                          const UAxeGameplayAbility* NewAxeAbility) const
{
	bool bBlocked = false;


	switch (Group)
	{
	case EAxeAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EAxeAbilityActivationGroup::Exclusive_Replaceable:
	case EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition:
		{
			const TArray<FGameplayAbilitySpecHandle>* SpecHandles = ActivationGroupMap.Find(
				EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition
			);
			if (SpecHandles)
			{
				for (const FGameplayAbilitySpecHandle& SpecHandle : *SpecHandles)
				{
					FGameplayAbilitySpec* GameplayAbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
					UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(GameplayAbilitySpec->Ability);
					if (!AxeGameplayAbility)
					{
						continue;
					}
					bool bReplaceByCondition = AxeGameplayAbility->CanReplaceAbilityByCondition(
						NewAxeAbility, GetAxeCharacterOwner()
					);
					if (!bReplaceByCondition)
					{
						bBlocked = true;
					}
				}
			}
		}
	case EAxeAbilityActivationGroup::Exclusive_Blocking:
		{
			const TArray<FGameplayAbilitySpecHandle>* SpecHandles = ActivationGroupMap.Find(
				EAxeAbilityActivationGroup::Exclusive_Blocking
			);
			if (SpecHandles && !bBlocked)
			{
				bBlocked = SpecHandles->Num() > 0;
			}
			break;
		}
	default:
		break;
	}

	return bBlocked;
}

void UAxeAbilitySystemComponent::AddAbilityToActivationGroup(EAxeAbilityActivationGroup Group,
                                                             UAxeGameplayAbility* AxeAbility)
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AxeAbility->GetClass());
	if (!AbilitySpec)
	{
		return;
	}
	FGameplayAbilitySpecHandle AxeAbilitySpecHandle = AbilitySpec->Handle;
	ActivationGroupMap.FindOrAdd(Group).AddUnique(AxeAbilitySpecHandle);

	switch (Group)
	{
	case EAxeAbilityActivationGroup::Independent:
		break;
	case EAxeAbilityActivationGroup::Exclusive_Replaceable:
	case EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition:
	case EAxeAbilityActivationGroup::Exclusive_Blocking:
		{
			TArray<FGameplayAbilitySpecHandle> CancelAbilityHandles;
			if (TArray<FGameplayAbilitySpecHandle>* AbilitySpecHandles = ActivationGroupMap.Find(
				EAxeAbilityActivationGroup::Exclusive_Replaceable))
			{
				CancelAbilityHandles.Append(*AbilitySpecHandles);
			}
			if (TArray<FGameplayAbilitySpecHandle>* AbilitySpecHandles = ActivationGroupMap.Find(
				EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition))
			{
				CancelAbilityHandles.Append(*AbilitySpecHandles);
			}
			if (CancelAbilityHandles.Num() > 0)
			{
				for (const FGameplayAbilitySpecHandle& Handle : CancelAbilityHandles)
				{
					if (Handle == AxeAbilitySpecHandle)
					{
						continue;
					}
					CancelAbilityHandle(Handle);
				}
			}
			break;
		}
	default:
		break;
	}
}

void UAxeAbilitySystemComponent::RemoveAbilityFromActivationGroup(EAxeAbilityActivationGroup Group,
                                                                  UAxeGameplayAbility* AxeAbility)
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AxeAbility->GetClass());
	if (!AbilitySpec)
	{
		return;
	}
	FGameplayAbilitySpecHandle AxeAbilitySpecHandle = AbilitySpec->Handle;
	ActivationGroupMap.FindOrAdd(Group).Remove(AxeAbilitySpecHandle);
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

	if (GetAxeCharacterOwner()->IsLocallyControlled())
	{
		OnNotifyAbilityActivatedDelegate.Broadcast(Ability);
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Ability Activated: %s"),
	                                                                         *Ability->GetName()));
}

void UAxeAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
                                                    bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability);
	RemoveAbilityFromActivationGroup(AxeGameplayAbility->GetActivationGroup(), AxeGameplayAbility);

	if (GetAxeCharacterOwner()->IsLocallyControlled())
	{
		OnNotifyAbilityEndedDelegate.Broadcast(Ability);
	}
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyEffectToSelfByClass(
	const TSubclassOf<UGameplayEffect>& EffectClass, const float Level)
{
	check(EffectClass);
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(GetAxeCharacterOwner());
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, Level, ContextHandle
	);
	return ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), this);
}
