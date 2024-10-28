// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/HitTraceAnimNotifyState.h"

#include "Character/AxeCharacterPlayer.h"

void UHitTraceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);
	if (AxeCharacterPlayer == nullptr)
	{
		return;
	}
}

void UHitTraceAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UHitTraceAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
