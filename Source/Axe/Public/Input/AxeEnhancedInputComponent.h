// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeInputDataAsset.h"
#include "EnhancedInputComponent.h"
#include "AxeEnhancedInputComponent.generated.h"

class UAxeInputDataAsset;
/**
 * 
 */
UCLASS()
class AXE_API UAxeEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAxeInputDataAsset* InputConfig, UserClass* UserObject, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAxeEnhancedInputComponent::BindAbilityActions(const UAxeInputDataAsset* InputConfig, UserClass* UserObject,
                                                    PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                                    HeldFuncType HeldFunc)
{
	for (const FAxeInputActionStruct& InputAction : InputConfig->AbilityInputActionList)
	{
		if (InputAction.InputTag.IsValid() && InputAction.InputAction)
		{
			if (PressedFunc)
			{
				BindAction(
					InputAction.InputAction, ETriggerEvent::Started, UserObject, PressedFunc, InputAction.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(
					InputAction.InputAction, ETriggerEvent::Completed, UserObject, ReleasedFunc, InputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(
					InputAction.InputAction, ETriggerEvent::Triggered, UserObject, HeldFunc, InputAction.InputTag);
			}
		}
	}
}
