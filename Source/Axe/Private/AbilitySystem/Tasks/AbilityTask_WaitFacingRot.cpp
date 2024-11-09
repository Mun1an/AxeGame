// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_WaitFacingRot.h"

#include "Character/AxeCharacterPlayer.h"

UAbilityTask_WaitFacingRot* UAbilityTask_WaitFacingRot::CreateWaitFacingRotTask(
	UGameplayAbility* OwningAbility, ACharacter* Character, FRotator TargetRot, float InterpSpeed)
{
	UAbilityTask_WaitFacingRot* MyObj = NewAbilityTask<UAbilityTask_WaitFacingRot>(OwningAbility);
	MyObj->AxeCharacterBase = Character;
	MyObj->TargetRot = TargetRot;
	MyObj->InterpSpeed = InterpSpeed;

	return MyObj;
}

void UAbilityTask_WaitFacingRot::Activate()
{
	Super::Activate();
	bTickingTask = true;
}

void UAbilityTask_WaitFacingRot::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	TotalTickTime += DeltaTime;
	if (TotalTickTime >= MaxTickTime)
	{
		EndTask();
		return;
	}

	if (AxeCharacterBase)
	{
		FRotator NowRot = AxeCharacterBase->GetActorRotation();
		if (abs(NowRot.Yaw - TargetRot.Yaw) > 1)
		{
			float NewYaw = FMath::FInterpTo(NowRot.Yaw, TargetRot.Yaw, DeltaTime, InterpSpeed);
			NowRot.Yaw = NewYaw;
			AxeCharacterBase->SetActorRotation(NowRot, ETeleportType::ResetPhysics);
		}
		else
		{
			EndTask();
		}
	}
}

void UAbilityTask_WaitFacingRot::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	bTickingTask = false;
}
