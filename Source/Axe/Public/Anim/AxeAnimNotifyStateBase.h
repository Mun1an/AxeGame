// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AxeAnimNotifyStateBase.generated.h"

class UAxeAbilitySystemComponent;
class AAxeCharacterPlayer;
class UComboActionComponent;
class AAxePlayerController;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class AXE_API UAxeAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	AAxeCharacterPlayer* GetPlayerCharacter(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	AAxePlayerController* GetPlayerController(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	UComboActionComponent* GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponent(const USkeletalMeshComponent* MeshComp);
};
