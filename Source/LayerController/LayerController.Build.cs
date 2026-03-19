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
        
        bUseRTTI = false;

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

        PrivateDependencyModuleNames.AddRange(new string[] { });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            
            "Utils",
            "GameOptions",
            "Weather",
            "CommonType",
            "StateProcessor",
            "GameplayTags",
            
            "LayerData",
            "LayerVisualize",
        });

        PublicDependencyModuleNames.AddRange(new string[] {
        });
    }
}
