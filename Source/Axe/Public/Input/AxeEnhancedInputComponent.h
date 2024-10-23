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
	for (const FAxeInputActionStruct& InputActionStruct : InputConfig->AbilityInputActionList)
	{
		if (InputActionStruct.InputTag.IsValid() && InputActionStruct.InputAction)
		{
			if (PressedFunc)
			{
				BindAction(
					InputActionStruct.InputAction, ETriggerEvent::Started, UserObject, PressedFunc, InputActionStruct.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(
					InputActionStruct.InputAction, ETriggerEvent::Completed, UserObject, ReleasedFunc, InputActionStruct.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(
					InputActionStruct.InputAction, ETriggerEvent::Triggered, UserObject, HeldFunc, InputActionStruct.InputTag);
			}
		}
	}
}
