// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyInfoBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UEnemyInfoBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UEnemyInfoBarWidgetComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfoBar")
	float ShowBarDistance = 900;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfoBar")
	float HideBarDistance = 1000;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfoBar")
	// bool bShowOnHurt = true;
};
