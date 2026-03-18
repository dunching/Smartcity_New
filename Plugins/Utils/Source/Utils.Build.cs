// Copyright 2019 Tefel. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class Utils : ModuleRules
{
	public Utils(ReadOnlyTargetRules Target) : base(Target)
    {
	    // Include What You Use (IWYU)
	    bUseUnity = false;
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        DefaultBuildSettings = BuildSettingsVersion.Latest;

        CppStandard = CppStandardVersion.Cpp20;

        bUseRTTI = false;

        PublicIncludePaths.AddRange(
			new string[] {
                   Path.Combine(PluginDirectory, "Source/Utils"),
                   Path.Combine(PluginDirectory, "Source/Utils/LogHelper"),
                   Path.Combine(PluginDirectory, "Source/Utils/ThreadPoolHelper"),
                   Path.Combine(PluginDirectory, "Source/Utils/BaseData"),
                   Path.Combine(PluginDirectory, "Source/Utils/ScopeEvent"),
                   Path.Combine(PluginDirectory, "Source/Utils/Language"),
                   Path.Combine(PluginDirectory, "Source/Utils/Algorithm"),
                   Path.Combine(PluginDirectory, "Source/Utils/KismetTools"),
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
            }
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
            }
			);

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
            {
            }
			);
	}
}
