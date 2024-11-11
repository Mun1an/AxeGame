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
	Ass_FrontSwing,
	ASS_Using,
	ASS_BackSwing
};
