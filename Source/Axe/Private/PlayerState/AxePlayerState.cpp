// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AxePlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Net/UnrealNetwork.h"


AAxePlayerState::AAxePlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAxeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAxeAttributeSet>("AxeAttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);

	InitializeLevelXpThresholds();
}

void AAxePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AAxePlayerState, PlayerLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AAxePlayerState, Xp, COND_None, REPNOTIFY_Always);
}

UAbilitySystemComponent* AAxePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UInventoryComponent* AAxePlayerState::GetInventoryComponent() const
{
	return InventoryComponent;
}

// Level
void AAxePlayerState::SetPlayerLevel(const float NewLevel, bool bCheckXp)
{
	PlayerLevel = FMath::Clamp(NewLevel, 0, MaxPlayerLevel);
	OnLevelChangedDelegate.Broadcast(PlayerLevel);

	//
	if (bCheckXp)
	{
		const int32 LevelXpThreshold = GetLevelXpThreshold(NewLevel);
		if (Xp < LevelXpThreshold)
		{
			SetPlayerXp(LevelXpThreshold);
		}
	}
}

void AAxePlayerState::AddToPlayerLevel(const float AddLevel)
{
	SetPlayerLevel(PlayerLevel + AddLevel);
}

// Xp
void AAxePlayerState::SetPlayerXp(const float NewXp)
{
	Xp = FMath::Max(NewXp, 0);
	OnXpChangedDelegate.Broadcast(Xp);

	// fixme 优化
	const int32 NewLevel = GetNewLevelByTotalXp(Xp);
	if (PlayerLevel != NewLevel)
	{
		SetPlayerLevel(NewLevel, false);
	}
}

void AAxePlayerState::AddToPlayerXp(const float AddXp)
{
	SetPlayerXp(Xp + AddXp);
}

int32 AAxePlayerState::GetNewLevelByTotalXp(int32 TotalXp) const
{
	// 等级计算
	for (int32 Level = 1; LevelXpThresholds.IsValidIndex(Level); ++Level)
	{
		if (TotalXp < LevelXpThresholds[Level])
		{
			return Level - 1;
		}
	}
	return LevelXpThresholds.Num() - 1;
}

int32 AAxePlayerState::GetOneNextLevelRequireXp(int32 FromLevel) const
{
	return (FromLevel - 1) * 100;
}

int32 AAxePlayerState::GetLevelXpThreshold(int32 Level) const
{
	if (LevelXpThresholds.IsValidIndex(Level))
	{
		return LevelXpThresholds[Level];
	}
	return LevelXpThresholds.Last();
}

float AAxePlayerState::GetXpPercent() const
{
	const int32 LevelXpThreshold = GetLevelXpThreshold(PlayerLevel);
	const int32 NextLevelXpThreshold = GetLevelXpThreshold(PlayerLevel + 1);
	float Per = static_cast<float>(Xp - LevelXpThreshold) / static_cast<float>(NextLevelXpThreshold - LevelXpThreshold);
	Per = Per >= 1 ? 0.f : Per;
	return Per;
}

void AAxePlayerState::OnRep_Level(int32 OldValue)
{
	OnLevelChangedDelegate.Broadcast(OldValue);
}

void AAxePlayerState::OnRep_Xp(int32 OldValue)
{
	OnXpChangedDelegate.Broadcast(OldValue);
}

void AAxePlayerState::InitializeLevelXpThresholds()
{
	LevelXpThresholds.SetNum(MaxPlayerLevel + 1);
	LevelXpThresholds[0] = 0;

	for (int32 Level = 1; Level <= MaxPlayerLevel; ++Level)
	{
		LevelXpThresholds[Level] = LevelXpThresholds[Level - 1] + GetOneNextLevelRequireXp(Level);
	}
}
