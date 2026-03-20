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

        PrivateDefinitions.Add("LOGQUERY = 0");
        PrivateDefinitions.Add("RECODETIME = 0");

        PrivateDefinitions.Add("USEFRONTENDDATA = 1");  

        PrivateDefinitions.Add("USEBACKENDDATA = 0");  

        PublicIncludePaths.Add("LayerData");
        PublicIncludePaths.Add("LayerData/LayerData");
        PublicIncludePaths.Add("LayerData/PIxelStreaming");
        PublicIncludePaths.Add("LayerData/QueryDeviceInfoSubSystem");
        PublicIncludePaths.Add("LayerData/LayerController/PixelStreaming/");

        PrivateDependencyModuleNames.AddRange(new string[] {
            // 引擎内容
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "SlateCore",
            "Slate",
            "UMG",
            "EnhancedInput",
            "AIModule",
            "StateTreeModule",
            "NavigationSystem",
            "ChaosVehicles",
            "ActorSequence",
            "NetCore",
            "ShaderConductor",
            "Json",
            "MediaAssets",
            "DatasmithContent",
            "MovieScene",
            
            // 引擎插件
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
            "GameplayStateTreeModule",
            "GameplayAbilities",
            "GameplayCameras",
            "Niagara",
            "PixelStreaming",
            "GeometryFramework",
            "HTTP",
            "WebSockets",
            "Sockets",
            "PixelStreaming",
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            
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
    }
}
