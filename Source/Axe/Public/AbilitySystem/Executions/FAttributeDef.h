#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

struct FAttributeDef
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	FAttributeDef()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Dexterity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, Intelligence, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, PhysicalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, MagicResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAxeAttributeSet, CriticalHitDamage, Source, false);
	}

};
