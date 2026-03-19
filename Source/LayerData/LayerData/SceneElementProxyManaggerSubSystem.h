// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SceneElementProxyManaggerSubSystem.generated.h"

class UDTMqttClient;

/**
 * 
 */
UCLASS()
class LAYERDATA_API USceneElementProxyManaggerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static USceneElementProxyManaggerSubSystem* GetInstance();
};
