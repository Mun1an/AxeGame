// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectComponent.h"
#include "AddStunEffectComponent.generated.h"


struct FGameplayEffectRemovalInfo;
struct FActiveGameplayEffectHandle;
class UGameplayAbility;

UCLASS()
class AXE_API UAddStunEffectComponent : public UGameplayEffectComponent
{
	friend class UGameplayEffect; // for upgrade path

	GENERATED_BODY()

public:
	/** Constructor */
	UAddStunEffectComponent();

	/** Register for the appropriate events when we're applied */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer,
	                                         FActiveGameplayEffect& ActiveGE) const override;

protected:

	/** This allows us to be notified when the owning GameplayEffect has had its inhibition changed (which can happen on the initial application). */
	virtual void OnInhibitionChanged(FActiveGameplayEffectHandle ActiveGEHandle, bool bIsInhibited) const;

	/** We should grant the configured Gameplay Abilities */
	virtual void StartAbilities(FActiveGameplayEffectHandle ActiveGEHandle) const;

	/** We should remove the configured Gameplay Abilities */
	virtual void EndAbilities(FActiveGameplayEffectHandle ActiveGEHandle) const;

	/** We must undo all effects when removed */
	void OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo) const;
};
