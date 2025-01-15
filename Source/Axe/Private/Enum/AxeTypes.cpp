#include "Enum/AxeTypes.h"

bool FAxeGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}

		// Add new RepBits here
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsEvasive)
		{
			RepBits |= 1 << 8;
		}
		if (bIsBlocked)
		{
			RepBits |= 1 << 9;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffChance > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 13;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 14;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 15;
		}
		if (DamageSpecialExpression != EDamageSpecialExpression::None)
		{
			RepBits |= 1 << 16;
		}
		if (!KnockbackVector.IsZero())
		{
			RepBits |= 1 << 17;
		}
		if (KnockbackForceMagnitude > 0.f)
		{
			RepBits |= 1 << 18;
		}
		if (PreDamageValue > 0.f)
		{
			RepBits |= 1 << 19;
		}
		if (DamageToToughnessValue > 0.f)
		{
			RepBits |= 1 << 20;
		}
	}

	Ar.SerializeBits(&RepBits, 20);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	// Add new RepBits here
	if (RepBits & (1 << 7))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsEvasive;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsBlocked;
	}
	if (RepBits & (1 << 10))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffChance;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 13))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 14))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 15))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		Ar << DamageSpecialExpression;
	}
	if (RepBits & (1 << 17))
	{
		KnockbackVector.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 18))
	{
		Ar << KnockbackForceMagnitude;
	}
	if (RepBits & (1 << 19))
	{
		Ar << PreDamageValue;
	}
	if (RepBits & (1 << 20))
	{
		Ar << DamageToToughnessValue;
	}

	//
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
