// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/IgnoreInputAnimNotifyState.h"


UIgnoreInputAnimNotifyState::UIgnoreInputAnimNotifyState()
{
	NotifyColor = FColor::Silver;
}

void UIgnoreInputAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
