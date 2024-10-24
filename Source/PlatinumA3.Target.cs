// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PlatinumA3Target : TargetRules
{
	public PlatinumA3Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("PlatinumA3");
		RegisterModulesCreatedByRider();
	}
	
	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] { "LocalMultiplayer" });
	}
}
