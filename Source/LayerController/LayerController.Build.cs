// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LayerController : ModuleRules
{
	public LayerController(ReadOnlyTargetRules Target) : base(Target)
    {
        bUseUnity = false;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
        bWarningsAsErrors = true;
        bEnableExceptions = true;

        CppStandard = CppStandardVersion.Cpp20;
        
        bUseRTTI = true;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
        }

        PublicIncludePaths.Add("LayerController/LayerController");
        PublicIncludePaths.Add("LayerController/Imp_InputProcessor");
        PublicIncludePaths.Add("LayerController/Imp_InputProcessor/InputProcessor");
        PublicIncludePaths.Add("LayerController/Imp_InputProcessor/InputProcessor/HumanProcessor");
        PublicIncludePaths.Add("LayerController/Imp_InputProcessor/InputProcessor/TourProcessor");
        PublicIncludePaths.Add("LayerController/SceneInteractionDecorator");
        PublicIncludePaths.Add("LayerController/PixelStreaming");
        PublicIncludePaths.Add("LayerController/GameplayTasks");
        PublicIncludePaths.Add("LayerController/GamePlay/Controller");

        PrivateDependencyModuleNames.AddRange(new string[] {
            // 引擎内容
            "Core",
            "CoreUObject",
            "Engine",
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            // 引擎插件
            "PixelStreaming",
            "Json",
            "GameplayTasks",
            "DatasmithContent",
            
            // 插件
            "Utils",
            "GameOptions",
            "Weather",
            "CommonType",
            "StateProcessor",
            "GameplayTags",
            "SmartCitySuite",
            "BuildingTools",
            "GameplayTaskHelper",
            
            // 其他模块
            "LayerData",
            "LayerVisualize",
            "AssetRef",
        });
    }
}
