// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LayerVisualize : ModuleRules
{
	public LayerVisualize(ReadOnlyTargetRules Target) : base(Target)
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

        PublicIncludePaths.Add("LayerVisualize");
        PublicIncludePaths.Add("LayerVisualize/LayerVisualize");
        PublicIncludePaths.Add("LayerVisualize/Marks");
        PublicIncludePaths.Add("LayerVisualize/Effects");
        PublicIncludePaths.Add("LayerVisualize/Effects/Buildings");
        PublicIncludePaths.Add("LayerVisualize/Effects/SceneElement");
        PublicIncludePaths.Add("LayerVisualize/Effects/SceneElementManagger");
        PublicIncludePaths.Add("LayerVisualize/Effects/Spaces");
        PublicIncludePaths.Add("LayerVisualize/ElevatorSubSystem");
        PublicIncludePaths.Add("LayerVisualize/UI");
        PublicIncludePaths.Add("LayerVisualize/UI/HUD");
        PublicIncludePaths.Add("LayerVisualize/BuildingTools");
        PublicIncludePaths.Add("LayerVisualize/SpaceSubSystem");
        PublicIncludePaths.Add("LayerVisualize/AssetRefMap");
        PublicIncludePaths.Add("LayerVisualize/GamePlay");
        PublicIncludePaths.Add("LayerVisualize/GamePlay/WorldSetting");

        PrivateDependencyModuleNames.AddRange(new string[] { });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
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
            
            // 插件
            "Common_UMG",
            "StateProcessor",
            "Utils",
            "Weather",
            "BuildingTools",
            "Effects",
            "SmartCitySuite",
            "AssetRef",
            
            // 其他模块
            "GameplayTags",
            "CommonType",
            "GameOptions",
        });

        PublicDependencyModuleNames.AddRange(new string[] {
        });
    }
}
