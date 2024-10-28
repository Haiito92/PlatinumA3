// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

using UnrealBuildTool;

public class SkyCreatorEditorPlugin : ModuleRules
{
	public SkyCreatorEditorPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[]
			{
				"SkyCreatorPlugin/Private",
				"SkyCreatorPlugin/Public",
                // ... add other private include paths required here ...
            }
            );
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				// ... add other public dependencies that you statically link with here ...
			}
			);

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "UnrealEd",
				"AssetTools",
				"PropertyEditor",
				"EditorStyle",
				"Projects",
                "SkyCreatorPlugin",
                "PlacementMode",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}