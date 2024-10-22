// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/AxeBaseActionComponent.h"
#include "ComboActionComponent.generated.h"

class UComboTreeNode;
class UComboTree;
class UComboDataAsset;
/**
 * 
 */
UCLASS()
class AXE_API UComboActionComponent : public UAxeBaseActionComponent
{
	GENERATED_BODY()

public:
	UComboActionComponent();

	UFUNCTION()
	void InitComboAbilityTree();
	//
	UPROPERTY(EditAnywhere, Category="Abilities|Combo")
	TObjectPtr<UComboDataAsset> ComboDataAsset;

	UComboTree* GetComboAbilityTree() const { return ComboAbilityTree; }
	
	//
	UPROPERTY()
	UComboTreeNode* LastComboTreeNode = nullptr;

	FGameplayTag GetCombo(FGameplayTag& NextInputAbilityTag);

	
	//
	UFUNCTION()
	void CombatWindowStart();

	UFUNCTION()
	void CombatWindowUpdate();

	UFUNCTION()
	void CombatWindowEnd();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UComboTree> ComboAbilityTree;
};
