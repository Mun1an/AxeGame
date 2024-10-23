// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AxeInputDataAsset.h"

const UInputAction* UAxeInputDataAsset::FindInputActionByInputTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAxeInputActionStruct& InputAction : AbilityInputActionList)
	{
		if (InputAction.InputAction && Tag == InputAction.InputTag)
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp, Error, TEXT("AbilityInputAction for tag [%s] not found in [%s]"),
			*Tag.ToString(),
			*GetNameSafe(this)
		);
	}

	return nullptr;
}
