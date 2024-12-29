// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OcTree_Test : ModuleRules
{
	public OcTree_Test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "RD", "RD" });
	}
}
