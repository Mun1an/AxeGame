// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AxeMotionWrapAnimNotifyState.h"

#include "Camera/CameraComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

UAxeMotionWrapAnimNotifyState::UAxeMotionWrapAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Turquoise;
#endif
}

void UAxeMotionWrapAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
