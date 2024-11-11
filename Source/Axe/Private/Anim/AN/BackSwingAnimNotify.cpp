// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN/BackSwingAnimNotify.h"

UBackSwingAnimNotify::UBackSwingAnimNotify()
{
	NotifyColor = FColor::White;
}

void UBackSwingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	
}
