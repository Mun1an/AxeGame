// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboAbilityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComboAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IComboAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState) = 0;
	UFUNCTION()
	virtual void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState) = 0;

	UFUNCTION()
	virtual void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState) = 0;
	UFUNCTION()
	virtual void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState) = 0;
};
