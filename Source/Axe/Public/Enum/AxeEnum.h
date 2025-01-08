#pragma once

UENUM(BlueprintType)
enum class ELaunchCharacterDirection: uint8
{
	Lc_Forward,
	Lc_Backward,
	Lc_Left,
	Lc_Right,
	Lc_Up,
	Lc_Down
};

UENUM(BlueprintType)
enum class EAbilitySkillStage: uint8
{
	ASS_FrontSwing,
	ASS_Using,
	ASS_BackSwing
};

UENUM(BlueprintType)
enum class EItemType: uint8
{
	None,
	Weapon,
	Item,
	Max
};

UENUM(BlueprintType)
enum class EHitTraceWeaponHandIndex : uint8
{
	WeaponHandIndex,
	WeaponSecondaryHandIndex
};
