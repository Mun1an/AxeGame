// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/CustomNameAnimNotifyState.h"


UCustomNameAnimNotifyState::UCustomNameAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Cyan;
#endif
}

void UCustomNameAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UCustomNameAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
