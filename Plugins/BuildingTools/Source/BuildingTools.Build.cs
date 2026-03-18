// Copyright 2019 Tefel. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class BuildingTools : ModuleRules
{
	public BuildingTools(ReadOnlyTargetRules Target) : base(Target)
    {
	    // Include What You Use (IWYU)
	    bUseUnity = false;
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        DefaultBuildSettings = BuildSettingsVersion.Latest;

        CppStandard = CppStandardVersion.Cpp20;

        bUseRTTI = false;

        PublicIncludePaths.AddRange(
			new string[] {
                   Path.Combine(PluginDirectory, "Source/BuildingTools"),
                   Path.Combine(PluginDirectory, "Source/CommonType"),
                   Path.Combine(PluginDirectory, "Source/SceneElement"),
                   Path.Combine(PluginDirectory, "Source/SceneElementManagger"),
                   Path.Combine(PluginDirectory, "Source/SceneElementTools"),
            }
			);
				
		PrivateIncludePaths.AddRange(
			new string[] {
            }
			);

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"Json",
				"DatasmithContent",
            }
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{

                "Effects",
                "SmartCitySuite", "Utils", 
            }
			);

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
            {
            }
			);
	}
}
