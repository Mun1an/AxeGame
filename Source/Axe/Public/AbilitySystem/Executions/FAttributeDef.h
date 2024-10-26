#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

struct FAttributeDef
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);

	FAttributeDef()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Dexterity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Intelligence, Source, false);
	}
};
