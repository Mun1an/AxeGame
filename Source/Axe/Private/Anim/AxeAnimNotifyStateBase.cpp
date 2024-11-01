// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AxeAnimNotifyStateBase.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "PlayerController/AxePlayerController.h"

UAxeAnimNotifyStateBase::UAxeAnimNotifyStateBase()
{
}

void UAxeAnimNotifyStateBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);
	
	if (AxeCharacterPlayer)
	{
		AnimNotifyStateBeginDelegate.Broadcast(this);
		if (AxeCharacterPlayer->IsLocallyControlled())
		{
			bIsNotifyStateEnded = false;
			bIsInterrupted = false;
		}
	}
}

void UAxeAnimNotifyStateBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp);

	if (AxeCharacterPlayer && AxeCharacterPlayer->IsLocallyControlled())
	{
		if (AxeCharacterPlayer->IsLocallyControlled())
		{
			AnimNotifyStateEndDelegate.Broadcast(this);
		}

		if (AxeCharacterPlayer->IsLocallyControlled())
		{
			bIsNotifyStateEnded = true;
			bIsInterrupted = false;
		}
	}
}

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

UAxeAbilitySystemComponent* UAxeAnimNotifyStateBase::GetAxeAbilitySystemComponent(
	const USkeletalMeshComponent* MeshComp)
{
	if (AAxeCharacterPlayer* AxeCharacterPlayer = GetPlayerCharacter(MeshComp))
	{
		return Cast<UAxeAbilitySystemComponent>(AxeCharacterPlayer->GetAbilitySystemComponent());
	}
	return nullptr;
}
