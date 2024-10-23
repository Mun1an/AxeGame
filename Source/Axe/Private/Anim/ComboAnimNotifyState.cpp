// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ComboAnimNotifyState.h"

#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "PlayerController/AxePlayerController.h"

AAxePlayerController* UComboAnimNotifyState::GetPlayerController(const USkeletalMeshComponent* MeshComp)
{
	APawn* Owner = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	AController* Controller = Cast<ACharacter>(Owner)->GetController();
	AAxePlayerController* AxePlayerController = Cast<AAxePlayerController>(Controller);
	return AxePlayerController;
}

UComboActionComponent* UComboAnimNotifyState::GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp)
{
	APawn* Pawn = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		const AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(Pawn);
		return CharacterPlayer->GetComboActionComponent();
	}
	return nullptr;
}

void UComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UComboActionComponent* ComboActionComponent = GetPlayerComboActionComponent(MeshComp))
	{
		ComboActionComponent->ComboSwitchWindowStart();
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
		ComboActionComponent->ComboSwitchWindowEnd();
	}
}
