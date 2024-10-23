// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/AxeBaseActionComponent.h"
#include "ComboActionComponent.generated.h"

class UAxeAbilitySystemComponent;
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

	UPROPERTY(EditAnywhere, Category="Abilities|Combo")
	TObjectPtr<UComboDataAsset> ComboDataAsset;

	UFUNCTION()
	void InitComboAbilityTree();
	//
	UComboTree* GetComboAbilityTree() const { return ComboAbilityTree; }
	UFUNCTION()
	UAxeGameplayAbility* GetActivatedComboAbility();
	TSubclassOf<UAxeGameplayAbility>* GetComboAbilityByInputTag(const FGameplayTag& NextInputAbilityTag);
	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponent() const;
	//
	void OnComboAbilityActivated(UGameplayAbility* Ability);
	//
	UFUNCTION()
	void AnsComboSwitchWindowStart();
	UFUNCTION()
	void AnsComboSwitchWindowTick();
	UFUNCTION()
	void AnsComboSwitchWindowEnd();
	//
	UFUNCTION()
	void AnsComboInputCacheStart();
	UFUNCTION()
	void AnsComboInputCacheTick();
	UFUNCTION()
	void AnsComboInputCacheEnd();

protected:
	UPROPERTY()
	UComboTreeNode* LastComboTreeNode = nullptr;

	virtual void BeginPlay() override;

	void OnAbilityInitOver();
	void OnNotifyAbilityActivated(UGameplayAbility* Ability);
	void OnAbilityInputTagPressed(const FGameplayTag InputTag);

private:
	UPROPERTY()
	TObjectPtr<AAxeCharacterPlayer> AxeCharacterPlayer;

	UPROPERTY()
	TObjectPtr<UComboTree> ComboAbilityTree;

	UPROPERTY()
	bool bIsInComboWindow = false;
	UPROPERTY()
	TObjectPtr<UAxeGameplayAbility> ComboSwitchWindowStartAbility = nullptr;

	UPROPERTY()
	TObjectPtr<UAxeGameplayAbility> ActivatedComboAbility = nullptr;

	bool bSaveComboInputAbilityTagCache = false;
	TArray<FGameplayTag> ComboInputAbilityTagCache;
};
