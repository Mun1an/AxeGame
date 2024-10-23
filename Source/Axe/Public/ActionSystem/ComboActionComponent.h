// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/AxeBaseActionComponent.h"
#include "ComboActionComponent.generated.h"

class UAxeGameplayAbility;
class UGameplayAbility;
class AAxeCharacterPlayer;
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

	TSubclassOf<UAxeGameplayAbility>* GetComboAbilityByInputTag(const FGameplayTag& NextInputAbilityTag);

	//
	UFUNCTION()
	void ComboSwitchWindowStart();

	UFUNCTION()
	void ComboSwitchWindowTick();

	UFUNCTION()
	void ComboSwitchWindowEnd();

protected:
	virtual void BeginPlay() override;
	
	void OnAbilityInitOver();
	void OnNotifyAbilityActivated(UGameplayAbility* Ability);
	

private:
	UPROPERTY()
	TObjectPtr<AAxeCharacterPlayer> AxeCharacterPlayer;

	UPROPERTY()
	TObjectPtr<UComboTree> ComboAbilityTree;

	UPROPERTY()
	bool bIsInComboWindow = false;
};
