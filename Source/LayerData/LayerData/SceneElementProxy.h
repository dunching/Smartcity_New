// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BuildingGenerateTypes.h"

#include "SceneElementProxy.generated.h"

class ASceneElementBase;
class ASceneElementManaggerBase;

USTRUCT()
struct LAYERDATA_API FSceneElementProxy
{
	GENERATED_BODY()

	FString DeviceID;

	FString SubID;

	FString BimID;
	
	TWeakObjectPtr<ASceneElementBase> SceneElementPtr = nullptr;
	
	TWeakObjectPtr<ASceneElementManaggerBase> SceneElementManaggerPtr = nullptr;
	
	FModelIndex ModelIndex;
};
