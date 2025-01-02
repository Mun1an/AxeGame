// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
 * 客户端逻辑
 */
UCLASS()
class AXE_API UComboActionComponent : public UAxeActionComponentBase
{
	GENERATED_BODY()

public:
	UComboActionComponent();
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category="Abilities|Combo")
	TObjectPtr<UComboDataAsset> DefaultComboDataAsset;

	UFUNCTION()
	void InitComboAbilityTree(const UComboDataAsset* InComboDataAsset);
	//
	UComboTree* GetComboAbilityTree() const { return ComboAbilityTree; }
	UFUNCTION()
	UAxeGameplayAbility* GetActivatedComboAbility();

	TSubclassOf<UAxeGameplayAbility>* GetNextComboAbilityByInputTag(const FGameplayTag& NextInputAbilityTag);
	bool IsNextComboAbility(const UGameplayAbility* Ability);

	bool IsInComboSwitchWindow() const { return bIsInComboWindow; }

	//
	void OnAbilityActivated(UGameplayAbility* Ability);
	void OnAbilityEnded(UGameplayAbility* Ability);
	//
	UFUNCTION()
	void AnsComboSwitchWindowStart(UAnimNotifyState* NotifyState);
	UFUNCTION()
	void AnsComboSwitchWindowEnd(UAnimNotifyState* NotifyState);
	//
	UFUNCTION()
	void AnsComboInputCacheStart();
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
	TObjectPtr<UAxeGameplayAbility> ActivatedComboAbility = nullptr;

	UPROPERTY()
	bool bSaveComboInputAbilityTagCache = false;

	UPROPERTY()
	TArray<FGameplayTag> ComboInputAbilityTagCache;
};
