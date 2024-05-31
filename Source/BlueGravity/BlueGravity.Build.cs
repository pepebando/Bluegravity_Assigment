// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlueGravity : ModuleRules
{
	public BlueGravity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
