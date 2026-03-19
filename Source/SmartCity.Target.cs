// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SmartCityTarget : TargetRules
{
	public SmartCityTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			bForceEnableRTTI = true;
		}

		 bOverrideBuildEnvironment = true;
		
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("LayerController");
		ExtraModuleNames.Add("LayerData");
		ExtraModuleNames.Add("LayerVisualize");
		ExtraModuleNames.Add("SmartCity");
	}
}
