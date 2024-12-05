// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterBase.h"
#include "AxeCharacterMob.generated.h"

class UBehaviorTree;
class AAxeAIController;
/**
 * 
 */
UCLASS()
class AXE_API AAxeCharacterMob : public AAxeCharacterBase
{
	GENERATED_BODY()

public:
	AAxeCharacterMob();
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AAxeAIController> AxeAIController;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
