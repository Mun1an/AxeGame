// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interface/InventoryInterface.h"
#include "AxePlayerState.generated.h"

class UAttributeSet;
class UInventoryComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateValueChanged, int32)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnServerLevelUp, int32 /*NewValue*/, int32 /*OldValue*/)

/**
 * 
 */
UCLASS()
class AXE_API AAxePlayerState : public APlayerState, public IAbilitySystemInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	AAxePlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UInventoryComponent* GetInventoryComponent() const override;

	// Level And Xp
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerLevel(const float NewLevel, bool bCheckXp = true);
	UFUNCTION(BlueprintCallable)
	void AddToPlayerLevel(const float AddLevel);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPlayerXp() const { return Xp; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerXp(const float NewXp);
	UFUNCTION(BlueprintCallable)
	void AddToPlayerXp(const float AddXp);

	//
	UFUNCTION(BlueprintCallable)
	int32 GetNewLevelByTotalXp(int32 TotalXp) const;

	// 每级升至下一级(FromLevel + 1)所需单级经验
	UFUNCTION(BlueprintCallable)
	int32 GetOneNextLevelRequireXp(int32 FromLevel) const;

	// 获取等级所需经验
	UFUNCTION(BlueprintCallable)
	int32 GetLevelXpThreshold(int32 Level) const;

	UFUNCTION(BlueprintCallable)
	float GetXpPercent() const;

	//
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGoldCoinCount() const { return GoldCoinCount; }

	UFUNCTION(BlueprintCallable)
	void SetGoldCoinCount(int32 NewGoldCoin);
	UFUNCTION(BlueprintCallable)
	void AddToGoldCoinCount(int32 AddGoldCoin);

	FOnPlayerStateValueChanged OnXpChangedDelegate;
	FOnPlayerStateValueChanged OnLevelChangedDelegate;
	FOnPlayerStateValueChanged OnGoldCoinCountChangedDelegate;

	FOnServerLevelUp OnServerLevelUpDelegate;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;

private:
	UPROPERTY(VisibleAnywhere)
	int32 MaxPlayerLevel = 30;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 PlayerLevel = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Xp)
	int32 Xp = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_GoldCoinCount)
	int32 GoldCoinCount = 0;

	// 升级所需总经验列表
	TArray<int32> LevelXpThresholds;

	UFUNCTION()
	void OnRep_Level(int32 NewValue);
	UFUNCTION()
	void OnRep_Xp(int32 NewValue);
	UFUNCTION()
	void OnRep_GoldCoinCount(int32 NewValue);

	void InitializeLevelXpThresholds();
};
