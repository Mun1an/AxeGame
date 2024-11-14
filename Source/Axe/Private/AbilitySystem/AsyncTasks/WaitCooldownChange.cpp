#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"


UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                                const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	// EffectTagCountChangedDelegate
	FOnGameplayEffectTagCountChanged& EffectTagCountChangedDelegate = AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved
	);
	EffectTagCountChangedDelegate.AddUObject(WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged);


	// OnActiveGameplayEffectAddedDelegateToSelf
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded
	);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}
	FOnGameplayEffectTagCountChanged EffectTagCountChangedDelegate = ASC->RegisterGameplayTagEvent(
		CooldownTag,
		EGameplayTagEventType::NewOrRemoved
	);
	EffectTagCountChangedDelegate.RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec,
                                              FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (!AssetTags.HasTagExact(CooldownTag) && !GrantedTags.HasTagExact(CooldownTag))
	{
		return;
	}
	FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		CooldownTag.GetSingleTagContainer()
	);
	TArray<float> EffectsTimeRemainingList = AbilitySystemComponent->GetActiveEffectsTimeRemaining(EffectQuery);

	if (EffectsTimeRemainingList.Num() <= 0)
	{
		return;
	}
	float RemainingTime = EffectsTimeRemainingList[0];
	for (const float TimeRemaining : EffectsTimeRemainingList)
	{
		if (TimeRemaining > RemainingTime)
		{
			RemainingTime = TimeRemaining;
		}
	}
	CooldownStart.Broadcast(RemainingTime);
}
