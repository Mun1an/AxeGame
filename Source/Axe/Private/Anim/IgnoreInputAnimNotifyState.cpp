// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/IgnoreInputAnimNotifyState.h"


UIgnoreInputAnimNotifyState::UIgnoreInputAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Silver;
#endif
}

void UIgnoreInputAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
