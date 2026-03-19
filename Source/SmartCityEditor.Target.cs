// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SmartCityEditorTarget : TargetRules
{
	public SmartCityEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		
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
		ExtraModuleNames.Add("EditTool");
	}
}
