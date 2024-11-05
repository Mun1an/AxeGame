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

DECLARE_MULTICAST_DELEGATE_OneParam(FAnimNotifyStateDelegate, UAnimNotifyState*);
/**
 * 
 */
UCLASS()
class AXE_API UAxeAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FAnimNotifyStateDelegate AnimNotifyStateBeginDelegate;
	FAnimNotifyStateDelegate AnimNotifyStateEndDelegate;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

	bool GetIsNotifyStateEnded() const { return bIsNotifyStateEnded; }
	bool GetIsInterrupted() const { return bIsInterrupted; }
	void SetIsInterrupted(bool bInterrupted) { bIsInterrupted = bInterrupted; }
	float GetNotifyStateDuration() const { return NotifyStateDuration; }

protected:
	UPROPERTY()
	float NotifyStateDuration = 0.f;

	bool bIsInterrupted = false;
	bool bIsNotifyStateEnded = true;

	UFUNCTION()
	AAxeCharacterPlayer* GetPlayerCharacter(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	AAxePlayerController* GetPlayerController(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	UComboActionComponent* GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponent(const USkeletalMeshComponent* MeshComp);
};
