// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN/AxeAnimNotifyBase.h"

#include "Character/AxeCharacterBase.h"

void UAxeAnimNotifyBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const AAxeCharacterBase* AxeCharacterBase = GetAxeCharacterBase(MeshComp);

	if (AxeCharacterBase)
	{
		AnimNotifyBeginDelegate.Broadcast(this);
	}
}

AAxeCharacterBase* UAxeAnimNotifyBase::GetAxeCharacterBase(const USkeletalMeshComponent* MeshComp)
{
	APawn* Pawn = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	if (Pawn != nullptr)
	{
		return Cast<AAxeCharacterBase>(Pawn);
	}
	return nullptr;
}
