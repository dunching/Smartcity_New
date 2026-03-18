// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SmartCityTarget : TargetRules
{
	public SmartCityTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		 bForceEnableRTTI = true;
		 bOverrideBuildEnvironment = true;
		
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SmartCity");
	}
}
