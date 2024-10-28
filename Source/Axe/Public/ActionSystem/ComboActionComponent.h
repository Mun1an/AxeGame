// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/AxeActionComponentBase.h"
#include "ComboActionComponent.generated.h"

class UGameplayEffect;
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
class AXE_API UComboActionComponent : public UAxeActionComponentBase
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
	bool IsNextComboAbility(const UGameplayAbility* Ability);

	bool IsInComboSwitchWindow() const { return bIsInComboWindow; }

	//
	void OnComboAbilityActivated(UGameplayAbility* Ability);
	void OnComboAbilityEnded(UGameplayAbility* Ability);
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

	void PressedComboInputInCache();


	//

protected:
	UPROPERTY()
	UComboTreeNode* LastComboTreeNode = nullptr;

	virtual void BeginPlay() override;

	virtual void OnAbilityInitOver() override;
	void OnNotifyAbilityActivated(UGameplayAbility* Ability);
	void OnNotifyAbilityEnded(UGameplayAbility* Ability);
	void OnAbilityInputTagPressed(const FGameplayTag InputTag);

private:
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
