// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlatinumA3 : ModuleRules
{
	public PlatinumA3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] { "Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"DeveloperSettings",
				"StateMachine",
				"FCTween",
				"AsyncLoadingScreen", "CinematicCamera", "CinematicCamera"
			});
		
		PrivateDependencyModuleNames.AddRange(new string[] { "LocalMultiplayer", "StateMachine", "FocusableWidgets", "AIModule"});
		
		PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore", "UMG"});
	}
}
