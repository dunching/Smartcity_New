// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SceneElementProxyManaggerSubSystem.generated.h"

class UDTMqttClient;

struct FSceneElementProxy;

/**
 * 
 */
UCLASS()
class LAYERDATA_API USceneElementProxyManaggerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static USceneElementProxyManaggerSubSystem* GetInstance();
	
	/**
	 * DeviceID
	 */
	TMap<FString, TSharedPtr<FSceneElementProxy>> DeviceIDMap;
	
	/**
	 * BimID
	 */
	TMap<FString, TSharedPtr<FSceneElementProxy>> BimIDMap;
};
