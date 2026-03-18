// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

using Internal;
using UnrealBuildTool;

public class DTMqtt : ModuleRules
{
	public DTMqtt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(
			new string[]
				{
					"Core"
				}
			);
		PrivateDependencyModuleNames.AddRange(
			new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore"
				}
			);

        PrivateIncludePaths.AddRange(
			new string[]
				{
				}
			);
        
			PublicIncludePaths.AddRange(
				new string[]
				{
					ModuleDirectory + "/DTMqtt/",
					ModuleDirectory + "/MqttC/",
					ModuleDirectory + "/MqttCPP/",
				}
			);

        Console.Write("/wwwww/");
        Console.Write(ModuleDirectory + "/MqttC/"); ;
        
		UndefinedIdentifierWarningLevel = WarningLevel.Off;
		bEnableExceptions = true;
		PrivateDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
		PrivateDefinitions.Add("WIN32_LEAN_AND_MEAN");
		PrivateDefinitions.Add("PAHO_MQTT_STATIC");
		PrivateDefinitions.Add("NOMINMAX");
        
		if (Target.Platform == UnrealTargetPlatform.Mac
		    || Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDefinitions.Add("OSX");
		}
	}
}
