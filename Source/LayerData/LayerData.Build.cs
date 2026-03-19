// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LayerData : ModuleRules
{
	public LayerData(ReadOnlyTargetRules Target) : base(Target)
    {
        bUseUnity = false;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
        bWarningsAsErrors = true;
        bEnableExceptions = true;

        CppStandard = CppStandardVersion.Cpp20;
        
        bUseRTTI = false;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
        }

        PublicIncludePaths.Add("LayerData");
        PublicIncludePaths.Add("LayerData/LayerData");
        PublicIncludePaths.Add("LayerData/PIxelStreaming");
        PublicIncludePaths.Add("LayerData/QueryDeviceInfoSubSystem");

        PrivateDependencyModuleNames.AddRange(new string[] { });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "PixelStreaming",
            
            // 插件
            "GameplayTags",
            "Common_UMG",
            "StateProcessor",
            "Utils",
            "Weather",
            "BuildingTools",
            "Effects",
            "SmartCitySuite",
			
            // 其他模块
            "AssetRef",
            "CommonType",
            "GameOptions",
            "GameplayTagsLibrary",
            "Tools", 
            "DTMqtt", 
        });

        PublicDependencyModuleNames.AddRange(new string[] {
        });
    }
}
