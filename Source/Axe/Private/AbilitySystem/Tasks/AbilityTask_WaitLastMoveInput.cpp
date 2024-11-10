// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_WaitLastMoveInput.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "Character/AxeCharacterPlayer.h"

UAbilityTask_WaitLastMoveInput* UAbilityTask_WaitLastMoveInput::CreateWaitLastMoveInputTask(
	UGameplayAbility* OwningAbility, AAxeCharacterPlayer* AxeCharacterPlayer)
{
	UAbilityTask_WaitLastMoveInput* MyObj = NewAbilityTask<UAbilityTask_WaitLastMoveInput>(OwningAbility);
	MyObj->AxeCharacterPlayer = AxeCharacterPlayer;
	return MyObj;
}

void UAbilityTask_WaitLastMoveInput::Activate()
{
	Super::Activate();

	if (IsLocallyControlled())
	{
		SendDataToServer_Client();
	}
	else
	{
		// Server
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		FAbilityTargetDataSetDelegate& AbilityTargetDataSetDelegate =
			AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey);

		AbilityTargetDataSetDelegate.AddUObject(
			this, &UAbilityTask_WaitLastMoveInput::OnDataReplicatedCallback_Server);

		bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_WaitLastMoveInput::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitLastMoveInput::SendDataToServer_Client()
{
	// Window
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// 把 InputVector 装进 DataHandle
	FVector InputVector = UAxeBlueprintFunctionLibrary::GetAxeLastMovementInputVector(AxeCharacterPlayer);

	FGameplayAbilityTargetDataHandle DataHandle;
	FHitResult HitResult;
	HitResult.Location = InputVector;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
	DataHandle.Add(Data);

	const FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	FPredictionKey ScopedPredictionKey = AbilitySystemComponent->ScopedPredictionKey;
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GameplayAbilitySpecHandle,
		ActivationPredictionKey,
		DataHandle,
		FGameplayTag(),
		ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnLastMoveInputDelegate.Broadcast(InputVector);
	}
}

void UAbilityTask_WaitLastMoveInput::OnDataReplicatedCallback_Server(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                                     FGameplayTag ActivationTag)
{
	// Consume
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		const FGameplayAbilityTargetData* GameplayAbilityTargetData = DataHandle.Get(0);
		if (GameplayAbilityTargetData && GameplayAbilityTargetData->HasHitResult())
		{
			FVector InputVecter = GameplayAbilityTargetData->GetHitResult()->Location;
			OnLastMoveInputDelegate.Broadcast(InputVecter);
		}
	}
}
