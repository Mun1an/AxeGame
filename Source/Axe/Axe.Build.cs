// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Axe : ModuleRules
{
	public Axe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UnLua",
			"GameplayTags", "GameplayTasks", "GameplayAbilities", "MotionWarping"
		});
	}
}