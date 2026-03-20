// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PixelStreamingUtils.generated.h"

UCLASS()
class LAYERCONTROLLER_API UPixelStreamingUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void InitializeDeserializeStrategies();
};
