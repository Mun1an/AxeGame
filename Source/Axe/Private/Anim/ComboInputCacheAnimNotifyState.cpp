// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ComboInputCacheAnimNotifyState.h"

#include "ActionSystem/ComboActionComponent.h"

void UComboInputCacheAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->AnsComboInputCacheStart();
	}
}

void UComboInputCacheAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->AnsComboInputCacheTick();
	}
}

void UComboInputCacheAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->AnsComboInputCacheEnd();
	}
}
