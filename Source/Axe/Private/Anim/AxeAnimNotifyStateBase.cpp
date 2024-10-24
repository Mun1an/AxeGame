// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AxeAnimNotifyStateBase.h"

#include "Character/AxeCharacterPlayer.h"
#include "PlayerController/AxePlayerController.h"

AAxeCharacterPlayer* UAxeAnimNotifyStateBase::GetPlayerCharacter(const USkeletalMeshComponent* MeshComp)
{
	APawn* Pawn = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	if (Pawn != nullptr)
	{
		return Cast<AAxeCharacterPlayer>(Pawn);
	}
	return nullptr;
}

AAxePlayerController* UAxeAnimNotifyStateBase::GetPlayerController(const USkeletalMeshComponent* MeshComp)
{
	if (AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp))
	{
		AController* Controller = AxeCharacterPlayer->GetController();
		if (Controller)
		{
			return Cast<AAxePlayerController>(Controller);
		}
	}
	return nullptr;
}

UComboActionComponent* UAxeAnimNotifyStateBase::GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp)
{
	if (AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp))
	{
		return AxeCharacterPlayer->GetComboActionComponent();
	}
	return nullptr;
}
