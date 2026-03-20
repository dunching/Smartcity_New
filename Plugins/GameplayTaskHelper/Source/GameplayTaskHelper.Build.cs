// Copyright 2019 Tefel. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class GameplayTaskHelper : ModuleRules
{
	public GameplayTaskHelper(ReadOnlyTargetRules Target) : base(Target)
    {
	    // Include What You Use (IWYU)
	    bUseUnity = false;
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        DefaultBuildSettings = BuildSettingsVersion.Latest;

		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
		bWarningsAsErrors = true;
		bEnableExceptions = true;

        CppStandard = CppStandardVersion.Cpp20;

        bUseRTTI = false;

        PublicIncludePaths.AddRange(
			new string[] {
                   Path.Combine(PluginDirectory, "Source/GameplayTaskHelper"),
                   Path.Combine(PluginDirectory, "Source/GameplayTaskHelper/GameplayTaskHelper"),
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
				// 引擎内容
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"Json",
				
				// 引擎插件
				"GameplayTasks",
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
