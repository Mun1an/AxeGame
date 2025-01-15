// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterMob.h"
#include "Interface/EnemyInterface.h"
#include "AxeCharacterEnemy.generated.h"

class UMobOverlayWidgetController;
struct FWidgetControllerParams;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AXE_API AAxeCharacterEnemy : public AAxeCharacterMob, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAxeCharacterEnemy();

	// IEnemyInterface
	virtual void SetCombatTarget(AActor* NewCombatTarget);
	virtual AActor* GetCombatTarget() const;
	virtual int32 GetLootXp() const override { return LootXp; };
	//
	virtual void OnDead() override;

protected:
	virtual void BeginPlay() override;

	UMobOverlayWidgetController* GetMobOverlayWidgetController(const FWidgetControllerParams& Params);

	virtual void MulticastDeath_Implementation(FVector InDeathImpulseVector) override;

	void SendLootToPlayers(const TArray<AActor*>& Players);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY()
	TObjectPtr<UMobOverlayWidgetController> MobOverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMobOverlayWidgetController> MobOverlayWidgetControllerClass;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditDefaultsOnly, Category="Enemy|Loot")
	int32 LootXp = 0;

	UPROPERTY(EditDefaultsOnly, Category="Enemy|Loot")
	int32 LootGoldCoinCount = 0;
};
