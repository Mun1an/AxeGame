// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterBase.h"
#include "AxeCharacterMob.generated.h"

struct FAbilityEndedData;
class UBehaviorTree;
class AAxeAIController;
/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API AAxeCharacterMob : public AAxeCharacterBase
{
	GENERATED_BODY()

public:
	AAxeCharacterMob();
	virtual void PossessedBy(AController* NewController) override;

	virtual int32 GetCharacterLevel() const override;
	virtual void SetCharacterLevel(int32 NewCharacterLevel) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnAbilityActivated(UGameplayAbility* Ability);
	virtual void OnAbilityEnded(UGameplayAbility* Ability);
	virtual void SetIsUsingBlockingAbility();

	virtual void OnDead() override;

	UPROPERTY(EditAnywhere, Category= "Character|Level")
	int32 CharacterLevel = 1;
	
	UPROPERTY()
	TObjectPtr<AAxeAIController> AxeAIController;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	int32 IsUsingBlockingAbilityCount = 0;
};
