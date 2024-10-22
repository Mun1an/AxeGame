// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxeCharacterBase.generated.h"

class UComboActionComponent;
class UComboDataAsset;
class UGameplayAbility;

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

	/*
	 * AbilitySystemComponent
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

};
