// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AxeAnimNotifyStateBase.h"

#include "Character/AxeCharacterPlayer.h"
#include "PlayerController/AxePlayerController.h"

AAxePlayerController* UAxeAnimNotifyStateBase::GetPlayerController(const USkeletalMeshComponent* MeshComp)
{
	APawn* Owner = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	AController* Controller = Cast<ACharacter>(Owner)->GetController();
	AAxePlayerController* AxePlayerController = Cast<AAxePlayerController>(Controller);
	return AxePlayerController;
}

UComboActionComponent* UAxeAnimNotifyStateBase::GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp)
{
	APawn* Pawn = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		const AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(Pawn);
		return CharacterPlayer->GetComboActionComponent();
	}
	return nullptr;
}