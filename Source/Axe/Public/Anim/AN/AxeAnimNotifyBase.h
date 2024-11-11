// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AxeAnimNotifyBase.generated.h"
class AAxeCharacterBase;
DECLARE_MULTICAST_DELEGATE_OneParam(FAnimNotifyDelegate, UAnimNotify*);
/**
 * 
 */
UCLASS()
class AXE_API UAxeAnimNotifyBase : public UAnimNotify
{
	GENERATED_BODY()

public:
	FAnimNotifyDelegate AnimNotifyBeginDelegate;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                               const FAnimNotifyEventReference& EventReference) override;
protected:
	UFUNCTION()
	AAxeCharacterBase* GetAxeCharacterBase(const USkeletalMeshComponent* MeshComp);
};
