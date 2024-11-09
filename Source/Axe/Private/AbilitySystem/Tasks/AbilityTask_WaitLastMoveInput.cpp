// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_WaitLastMoveInput.h"

#include "Character/AxeCharacterPlayer.h"

UAbilityTask_WaitLastMoveInput* UAbilityTask_WaitLastMoveInput::CreateWaitLastMoveInputTask(
	UGameplayAbility* OwningAbility, ACharacter* AxeCharacterPlayer)
{
	UAbilityTask_WaitLastMoveInput* MyObj = NewAbilityTask<UAbilityTask_WaitLastMoveInput>(OwningAbility);
	MyObj->AxeCharacterPlayer = AxeCharacterPlayer;
	return MyObj;
}

void UAbilityTask_WaitLastMoveInput::Activate()
{
	Super::Activate();
	bTickingTask = true;
}

void UAbilityTask_WaitLastMoveInput::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (AxeCharacterPlayer->GetLastMovementInputVector() != FVector::ZeroVector)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnLastMoveInputDelegate.Broadcast();
			OnLastMoveInputDelegate.Clear();
			EndTask();
		}
	}
}

void UAbilityTask_WaitLastMoveInput::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	bTickingTask = false;
}
