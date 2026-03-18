// Copyright 2019 Tefel. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class Weather : ModuleRules
{
	public Weather(ReadOnlyTargetRules Target) : base(Target)
    {
	    bUseUnity = false;
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

	    CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
	    
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        CppStandard = CppStandardVersion.Cpp20;

        bUseRTTI = false;

        PublicIncludePaths.AddRange(
			new string[] {
                   Path.Combine(PluginDirectory, "Source/WeatherSystem"),
            }
			);
				
		PrivateIncludePaths.AddRange(
			new string[] {
                   Path.Combine(PluginDirectory, "Source/Command"),
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
            }
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "GameplayTags", 
                
                "Utils",
            }
			);

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
            {
            }
			);
	}
}
