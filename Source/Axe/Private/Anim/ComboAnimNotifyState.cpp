// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ComboAnimNotifyState.h"

#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "PlayerController/AxePlayerController.h"



void UComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->AnsComboSwitchWindowStart();
	}
}

void UComboAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->AnsComboSwitchWindowEnd();
	}
}
