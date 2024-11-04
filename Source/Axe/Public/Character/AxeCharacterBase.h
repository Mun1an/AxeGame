// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxeCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
enum class ELaunchCharacterDirection : uint8;
class UComboDataAsset;
class UGameplayAbility;

DECLARE_MULTICAST_DELEGATE(FOnAbilityInitOverDelegate);

UCLASS()
class AXE_API AAxeCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAxeCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/*
	 * AbilitySystemComponent
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// Attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffect;

	//
	FOnAbilityInitOverDelegate OnAbilityInitOverDelegate;
	bool IsAbilityInitOver() const { return bIsAbilityInitOver; }

	//
	

	//
	UFUNCTION()
	void SetWalkSpeed(float NewMaxWalkSpeed);
	UFUNCTION()
	void SetRotationRateZ(float NewRotationRateZ);
	UFUNCTION()
	void SetRotationRateByWalkSpeed();

protected:
	bool bIsAbilityInitOver = false;

	void InitDefaultAttributes();

	UPROPERTY()
	float MaxWalkSpeed = 1500.0f;
	UPROPERTY()
	float MinWalkSpeed = 0.0f;
	UPROPERTY()
	float MaxRotationRateZ = 800.0f;
	UPROPERTY()
	float MinRotationRateZ = 0.0f;
	UPROPERTY()
	float DefaultWalkSpeed = 600.0f;
};
