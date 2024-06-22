// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5TPSOnline : ModuleRules
{
	public UE5TPSOnline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
