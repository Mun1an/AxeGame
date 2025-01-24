// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "AbilitySystem/Ability/HitReact/HitReactBase.h"
#include "AbilitySystem/Executions/DamageExecution.h"
#include "ActionSystem/ComboActionComponent.h"
#include "AssetManager/AxeAssetManager.h"
#include "AssetManager/AxeGameData.h"
#include "Character/AxeCharacterPlayer.h"
#include "Enum/AxeTypes.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Item/Instance/EquipmentItemDefinition.h"
#include "Item/Instance/EquipmentItemInstance.h"


UAxeAbilitySystemComponent::UAxeAbilitySystemComponent()
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

void UAxeAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
		                      Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UAxeAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
		                      Spec.ActivationInfo.GetActivationPredictionKey());
	}
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
				TryActivateAbilityAndCheck_Client(AbilitySpec.Handle, InputTag);
			}
		}
	}
	OnAbilityInputTagPressedDelegate.Broadcast(InputTag);
}

void UAxeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// TODO
	TArray<FGameplayAbilitySpec>& GameplayAbilitySpecList = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				if (Cast<UAxeGameplayAbility>(AbilitySpec.Ability)->GetCanActiveByInputHeld())
				{
					AbilitySpecInputPressed(AbilitySpec);
					TryActivateAbilityAndCheck_Client(AbilitySpec.Handle, InputTag);
				}
			}
		}
	}
}

void UAxeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	TArray<FGameplayAbilitySpec>& GameplayAbilitySpecList = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAxeAbilitySystemComponent::TryActivateAbilityAndCheck_Client(FGameplayAbilitySpecHandle AbilitySpecHandle,
                                                                   const FGameplayTag InputTag,
                                                                   bool bAllowRemoteActivation)
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	UAxeGameplayAbility* AxeGameplayAbilityCDO = Cast<UAxeGameplayAbility>(AbilitySpec->Ability);
	AxeGameplayAbilityCDO->HandleTryActivateAbilityClientCDO(AbilitySpecHandle, InputTag, GetAxeCharacterOwner());

	TryActivateAbility(AbilitySpecHandle, bAllowRemoteActivation);
}

void UAxeAbilitySystemComponent::TryActivateHitReactAbility(const FGameplayTag HitReactTag, const FHitResult& HitResult,
                                                            AActor* SourceActor)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(HitReactTag);
	TArray<FGameplayAbilitySpec*> GameplayAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, GameplayAbilitySpecs);
	for (FGameplayAbilitySpec* GameplayAbilitySpec : GameplayAbilitySpecs)
	{
		for (UGameplayAbility* AbilityInstance : GameplayAbilitySpec->GetAbilityInstances())
		{
			if (UHitReactBase* HitReactBase = Cast<UHitReactBase>(AbilityInstance))
			{
				HitReactBase->SetHitResult(HitResult);
				HitReactBase->SetSourceActor(SourceActor);
			}
		}
		TryActivateAbility(GameplayAbilitySpec->Handle);
	}
}

bool UAxeAbilitySystemComponent::GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability,
                                                              const int32 AbilityLevel)
{
	const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GiveAbility(AbilitySpec);

	//
	return GameplayAbilitySpecHandle.IsValid();
}

void UAxeAbilitySystemComponent::GetAbilitySpecHandlesByActivationGroup(TArray<FGameplayAbilitySpecHandle>& HandleArray,
                                                                        EAxeAbilityActivationGroup Group) const
{
	HandleArray = ActivationGroupMap.FindRef(Group);
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
			if (SpecHandles && !bBlocked)
			{
				bBlocked = SpecHandles->Num() > 0;
			}
			break;
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
		if (AxeGameplayAbility->InputTagContainer.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AppendTags(AxeGameplayAbility->InputTagContainer);
		}
	}
	AbilitiesGivenDelegate.Broadcast();
}

void UAxeAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
                                                        UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability);
	AddAbilityToActivationGroup(AxeGameplayAbility->GetActivationGroup(), AxeGameplayAbility);

	const AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		OnNotifyAbilityActivatedDelegate.Broadcast(Ability);
	}
}

void UAxeAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
                                                    bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(Ability);
	RemoveAbilityFromActivationGroup(AxeGameplayAbility->GetActivationGroup(), AxeGameplayAbility);

	const AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		OnNotifyAbilityEndedDelegate.Broadcast(Ability);
	}
}

void UAxeAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	TryActivateAbilitiesOnSpawn();
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

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyAddAttrDurationEffectByTag(AActor* SourceActor,
	FGameplayTag AttributeTag, float AttributeValue, float Duration)
{
	const TSubclassOf<UGameplayEffect> EffectClass = UAxeAssetManager::GetSubclass(
		UAxeGameData::Get().GE_AddAttr_Duration_SetByCaller);
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, AttributeTag, AttributeValue
	);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, FAxeGameplayTags::Get().Effect_Magnitude_Duration,
		Duration
	);
	FActiveGameplayEffectHandle GameplayEffectSpecToSelf = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return GameplayEffectSpecToSelf;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyEffect(TSubclassOf<UGameplayEffect> EffectClass,
	AActor* SourceActor, AActor* TargetActor, float Level, float Duration)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, Level, ContextHandle
	);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	if (Spec)
	{
		Spec->SetSetByCallerMagnitude(FAxeGameplayTags::Get().Effect_Magnitude_Duration, Duration);
	}
	FActiveGameplayEffectHandle EffectHandle;
	if (GetAxeCharacterOwner() == TargetActor)
	{
		EffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	else
	{
		EffectHandle = ApplyGameplayEffectSpecToTarget(
			*SpecHandle.Data.Get(),
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
		);
	}
	return EffectHandle;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyEffectSetByCaller(TSubclassOf<UGameplayEffect> EffectClass,
                                                                               AActor* SourceActor, AActor* TargetActor,
                                                                               float Level, float Duration,
                                                                               const TMap<FGameplayTag, float>&
                                                                               SetByCallerMap)

{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, Level, ContextHandle
	);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	if (Spec)
	{
		Spec->SetSetByCallerMagnitude(FAxeGameplayTags::Get().Effect_Magnitude_Duration, Duration);
		for (const TTuple<FGameplayTag, float>& Pair : SetByCallerMap)
		{
			Spec->SetSetByCallerMagnitude(Pair.Key, Pair.Value);	
		}
	}
	
	FActiveGameplayEffectHandle EffectHandle;
	if (GetAxeCharacterOwner() == TargetActor)
	{
		EffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	else
	{
		EffectHandle = ApplyGameplayEffectSpecToTarget(
			*SpecHandle.Data.Get(),
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
		);
	}
	return EffectHandle;
}

bool UAxeAbilitySystemComponent::ApplyDamageEffect(AActor* SourceActor, AActor* TargetActor,
                                                   const FDamageEffectParams& Params)
{
	checkf(Params.DamageEffectClass, TEXT("DamageEffectClass is nullptr"));
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	ContextHandle.AddHitResult(Params.HitResult);
	FAxeGameplayEffectContext& AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		ContextHandle
	);
	AxeEffectContext.SetKnockbackVector(Params.KnockbackVector);
	AxeEffectContext.SetKnockbackForceMagnitude(Params.KnockbackForceMagnitude);
	AxeEffectContext.SetDamageToToughnessValue(Params.DamageToToughnessValue);

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		Params.DamageEffectClass, Params.EffectLevel, ContextHandle
	);
	// Tag
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, Params.DamageType, Params.DamageValue
	);

	const AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	if (AxeCharacterOwner == TargetActor)
	{
		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	else
	{
		ApplyGameplayEffectSpecToTarget(
			*SpecHandle.Data.Get(),
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
		);
	}
	return true;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyEquipmentEffectToSelf(
	const TSubclassOf<UGameplayEffect>& EffectClass,
	const TArray<FEquipmentInstanceAttributeInfo>& EquipmentAttributeInfos,
	FGameplayTag ItemTypeTag)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();

	ContextHandle.AddSourceObject(AxeCharacterOwner);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);
	for (const FEquipmentInstanceAttributeInfo& InstanceAttributeInfo : EquipmentAttributeInfos)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, InstanceAttributeInfo.AttributeTag, InstanceAttributeInfo.AttributeValue
		);
	}
	FActiveGameplayEffectHandle GameplayEffectSpecToSelf = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return GameplayEffectSpecToSelf;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyIncomingXpEffect(AActor* SourceActor, int32 XpValue)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);

	const TSubclassOf<UGameplayEffect> EffectClass = UAxeAssetManager::GetSubclass(
		UAxeGameData::Get().GE_IncomingXp_SetByCaller);

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, FAxeGameplayTags::Get().Effect_Magnitude_IncomingXp, XpValue
	);
	FActiveGameplayEffectHandle GameplayEffectSpecToSelf = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return GameplayEffectSpecToSelf;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyIncomingGoldCoinCountEffect(AActor* SourceActor,
	int32 GoldCoinCount)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);

	const TSubclassOf<UGameplayEffect> EffectClass = UAxeAssetManager::GetSubclass(
		UAxeGameData::Get().GE_IncomingGoldCoinCount_SetByCaller);

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, FAxeGameplayTags::Get().Effect_Magnitude_IncomingGoldCoinCount, GoldCoinCount
	);
	FActiveGameplayEffectHandle GameplayEffectSpecToSelf = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return GameplayEffectSpecToSelf;
}

FActiveGameplayEffectHandle UAxeAbilitySystemComponent::ApplyToughnessRecoverStopEffect(AActor* SourceActor)
{
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);

	const TSubclassOf<UGameplayEffect> EffectClass = UAxeAssetManager::GetSubclass(
		UAxeGameData::Get().GE_ToughnessRecoverStop);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);
	FActiveGameplayEffectHandle GameplayEffectSpecToSelf = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return GameplayEffectSpecToSelf;
}

void UAxeAbilitySystemComponent::ExecuteDelegateToGetAbilitySpec(const FAbilitySpecDataDelegate& Delegate)
{
	FScopedAbilityListLock AbilityLock(*this);

	const TArray<FGameplayAbilitySpec> GameplayAbilitySpecs = GetActivatableAbilities();
	for (const FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecs)
	{
		bool bExecuteIfBound = Delegate.ExecuteIfBound(AbilitySpec);
	}
}

FGameplayTag UAxeAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		FGameplayTagContainer& GameplayTags = AbilitySpec.Ability.Get()->AbilityTags;
		for (const FGameplayTag& Tag : GameplayTags)
		{
			FGameplayTag RequestGameplayTag = FGameplayTag::RequestGameplayTag(FName("Ability"));
			if (RequestGameplayTag.IsValid() && Tag.MatchesTag(RequestGameplayTag))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAxeAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(AbilitySpec.Ability);
		return AxeGameplayAbility->InputTag;
	}
	return FGameplayTag();
}

void UAxeAbilitySystemComponent::OnCharacterASCInitOverCallback()
{
}

FGameplayAbilitySpecHandle UAxeAbilitySystemComponent::GetAbilityHandleByAbilityTag(const FGameplayTag& AbilityTag)
{
	TArray<FGameplayAbilitySpec>& GameplayAbilitySpecs = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecs)
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag))
		{
			return AbilitySpec.Handle;
		}
	}
	return FGameplayAbilitySpecHandle();
}

FGameplayTag UAxeAbilitySystemComponent::GetAbilityTag(const UGameplayAbility* Ability)
{
	if (Ability->AbilityTags.Num() == 0)
	{
		return FGameplayTag::EmptyTag;
	}
	return Ability->AbilityTags.GetByIndex(0);
}

FGameplayTag UAxeAbilitySystemComponent::GetCooldownTagsByAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	FGameplayAbilitySpec* AbilitySpecFromHandle = FindAbilitySpecFromHandle(Handle);
	const FGameplayTagContainer* CdTags = AbilitySpecFromHandle->Ability->GetCooldownTags();
	if (CdTags)
	{
		return CdTags->First();
	}
	return FGameplayTag::EmptyTag;
}

UGameplayAbility* UAxeAbilitySystemComponent::GetActiveAbilityByTag(const FGameplayTag& Tag)
{
	const TArray<FGameplayAbilitySpec>& GameplayAbilitySpecs = GetActivatableAbilities();
	for (const FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecs)
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(Tag) && AbilitySpec.Ability->IsActive())
		{
			return AbilitySpec.Ability;
		}
	}
	return nullptr;
}

UDamageExecution* UAxeAbilitySystemComponent::GetEffectDamageExecutionCDO()
{
	UObject* Object = UDamageExecution::StaticClass()->ClassDefaultObject;
	if (Object)
	{
		return Cast<UDamageExecution>(Object);
	}
	return nullptr;
}


void UAxeAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	AbilitiesGivenDelegate.Broadcast();
}

void UAxeAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UAxeGameplayAbility* LyraAbilityCDO = Cast<UAxeGameplayAbility>(AbilitySpec.Ability))
		{
			LyraAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}
