// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN/BackSwingAnimNotify.h"

UBackSwingAnimNotify::UBackSwingAnimNotify(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::White;
#endif
}

void UBackSwingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
}
