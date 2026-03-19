// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmartCity : ModuleRules
{
	public SmartCity(ReadOnlyTargetRules Target) : base(Target)
	{
		// Include What You Use (IWYU)
		bUseUnity = false;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		DefaultBuildSettings = BuildSettingsVersion.Latest;

		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
		bWarningsAsErrors = true;
		bEnableExceptions = true;

		CppStandard = CppStandardVersion.Cpp20;

		bUseRTTI = true;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
		} 

		PublicDefinitions.Add("TEST_RADAR = 0");  
		PublicDefinitions.Add("TEST_AIRQUALITY = 0");  
		PublicDefinitions.Add("TEST_ELEVATOR = 1");  
		PublicDefinitions.Add("TEST_RADARLOG = 0");  

		PublicIncludePaths.Add("SmartCity");
		PublicIncludePaths.Add("SmartCity/OpenWorld");
		PublicIncludePaths.Add("SmartCity/GamePlay");
		PublicIncludePaths.Add("SmartCity/GamePlay/Controller");
		PublicIncludePaths.Add("SmartCity/GamePlay/Controller/Player");
		PublicIncludePaths.Add("SmartCity/GamePlay/Controller/NPC");
		PublicIncludePaths.Add("SmartCity/GamePlay/Component");
		PublicIncludePaths.Add("SmartCity/GamePlay/GameInstance");
		PublicIncludePaths.Add("SmartCity/GamePlay/GameMode");
		PublicIncludePaths.Add("SmartCity/GamePlay/GameState");
		PublicIncludePaths.Add("SmartCity/GamePlay/PlayerState");
		PublicIncludePaths.Add("SmartCity/GamePlay/Pawn");
		PublicIncludePaths.Add("SmartCity/GamePlay/Pawn/Human");
		PublicIncludePaths.Add("SmartCity/GamePlay/PlayerCameraManager");
		PublicIncludePaths.Add("SmartCity/GamePlay/GameViewportClient");
		PublicIncludePaths.Add("SmartCity/Command");
		PublicIncludePaths.Add("SmartCity/Common");

		
		if (Target.bBuildEditor == true)
		{
		}

		PublicDependencyModuleNames.AddRange(new string[]
		{
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

			"HTTP",
			"WebSockets",
			"XmlParser",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// 插件
			"Common_UMG",
			"StateProcessor",
			"Utils",
			"Weather",
			"BuildingTools",
			"Effects",
			"SmartCitySuite",
			
			// 其他模块
			"CommonType",
			"GameOptions",
			"GameplayTagsLibrary",
			"Tools", 
			"DTMqtt", 
			"LayerVisualize",
			"LayerData",
			"LayerController",
		});

		PrivateIncludePathModuleNames.AddRange(new string[]
		{
		});

		PublicIncludePathModuleNames.AddRange(new string[]
		{
		});
	}
}