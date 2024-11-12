// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterMob.h"
#include "AxeCharacterEnemy.generated.h"

class UMobOverlayWidgetController;
struct FWidgetControllerParams;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AXE_API AAxeCharacterEnemy : public AAxeCharacterMob
{
	GENERATED_BODY()

public:
	AAxeCharacterEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	// UPROPERTY()
	// TObjectPtr<UMobOverlayWidgetController> MobOverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMobOverlayWidgetController> MobOverlayWidgetControllerClass;
	UMobOverlayWidgetController* GetMobOverlayWidgetController(const FWidgetControllerParams& Params);
	
	virtual void MulticastDeath_Implementation(const FVector DeathImpulse) override;
};
