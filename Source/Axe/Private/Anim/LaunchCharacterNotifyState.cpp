// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/LaunchCharacterNotifyState.h"

#include "Character/AxeCharacterPlayer.h"

void ULaunchCharacterNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->SetCustomLaunchCharacter(
			LaunchSpeed, LaunchDirection, bXYOverride, bZOverride
		);
	}
}

void ULaunchCharacterNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
