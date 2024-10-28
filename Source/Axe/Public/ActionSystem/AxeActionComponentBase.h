// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AxeActionComponentBase.generated.h"


class UAxeAbilitySystemComponent;
class AAxeCharacterPlayer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UAxeActionComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UAxeActionComponentBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AAxeCharacterPlayer> AxeCharacterPlayer;

	UPROPERTY()
	TObjectPtr<UAxeAbilitySystemComponent> AxeAbilitySystemComponent;

	UFUNCTION()
	virtual void OnAbilityInitOver();

};
