// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AxeAnimNotifyStateBase.generated.h"

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
	AAxePlayerController* GetPlayerController(const USkeletalMeshComponent* MeshComp);
	UComboActionComponent* GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp);
};
