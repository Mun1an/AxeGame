// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DeadInterface.h"
#include "AxeCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
enum class ELaunchCharacterDirection : uint8;
class UComboDataAsset;
class UGameplayAbility;

DECLARE_MULTICAST_DELEGATE(FOnAbilityInitOverDelegate);
DECLARE_MULTICAST_DELEGATE(FOnInventoryInitOverDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDeadDelegate, AActor*, DeadActor);

UCLASS()
class AXE_API AAxeCharacterBase : public ACharacter, public IAbilitySystemInterface, public IDeadInterface
{
	GENERATED_BODY()

public:
	AAxeCharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> LevelUpPrimaryAttributesEffect;
	//
	FOnAbilityInitOverDelegate OnAbilityInitOverDelegate;
	FOnInventoryInitOverDelegate OnInventoryInitOverDelegate;
	bool IsAbilityInitOver() const { return bIsAbilityInitOver; }
	bool IsInventoryInitOver() const { return bIsInventoryInitOver; }
	//

	//
	UFUNCTION(BlueprintCallable)
	float GetDefaultWalkSpeed() const { return DefaultWalkSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetWalkSpeed(float NewMaxWalkSpeed);
	//
	virtual bool IsDead() const override { return bIsDead; }
	virtual bool IsAlive() const override { return !bIsDead; }
	//
	UFUNCTION(BlueprintCallable)
	void SetDeath();

	virtual void OnDead() override;

	FOnActorDeadDelegate OnActorDeadDelegate;

	void SetDeathImpulseVector(FVector NewDeathImpulseVector) { DeathImpulseVector = NewDeathImpulseVector; }

	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetCharacterLevel(int32 NewCharacterLevel);
	//

protected:
	bool bIsAbilityInitOver = false;
	bool bIsInventoryInitOver = false;

	void InitDefaultAttributes();
	bool GiveStartupAbilities();

	UPROPERTY(EditAnywhere, Category= "Axe Movement")
	float DefaultWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category= "Axe Movement")
	float MaxWalkSpeed = 1500.0f;
	UPROPERTY(EditAnywhere, Category= "Axe Movement")
	float MinWalkSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category= "Axe Movement")
	float MaxRotationRateZ = 2000.0f;
	UPROPERTY(EditAnywhere, Category= "Axe Movement")
	float MinRotationRateZ = 0.0f;

	bool bIsDead = false;
	UPROPERTY(EditAnywhere, Category="Combat")
	float DeadLifeSpan = 3.0f;
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastDeath(FVector InDeathImpulseVector);

private:
	FVector DeathImpulseVector = FVector::Zero();
};
