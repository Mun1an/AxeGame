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

DECLARE_MULTICAST_DELEGATE(FAnimNotifyStateDelegate);
/**
 * 
 */
UCLASS()
class AXE_API UAxeAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FAnimNotifyStateDelegate AnimNotifyStateBeginDelegate;
	FAnimNotifyStateDelegate AnimNotifyStateTickDelegate;
	FAnimNotifyStateDelegate AnimNotifyStateEndDelegate;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

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
