// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_WaitMoveInput.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "Character/AxeCharacterPlayer.h"

UAbilityTask_WaitMoveInput* UAbilityTask_WaitMoveInput::CreateWaitMoveInputTask(
	UGameplayAbility* OwningAbility, AAxeCharacterPlayer* AxeCharacterPlayer)
{
	UAbilityTask_WaitMoveInput* MyObj = NewAbilityTask<UAbilityTask_WaitMoveInput>(OwningAbility);
	MyObj->AxeCharacterPlayer = AxeCharacterPlayer;
	return MyObj;
}

void UAbilityTask_WaitMoveInput::Activate()
{
	Super::Activate();
	bTickingTask = true;
}

void UAbilityTask_WaitMoveInput::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (!AxeCharacterPlayer)
	{
		return;
	}
	
	const FVector LastMoveInput = AxeCharacterPlayer->GetLastMovementInputVector();
	if (LastMoveInput == FVector::ZeroVector)
	{
		return;
	}
	OnMoveInputDelegate.Broadcast(LastMoveInput);
	
	EndTask();
}

void UAbilityTask_WaitMoveInput::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}
