// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/LaunchCharacterNotifyState.h"

#include "Character/AxeCharacterPlayer.h"

void ULaunchCharacterNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	StateDuration = TotalDuration;
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
