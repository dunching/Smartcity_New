// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Vehicle.generated.h"

class USplineComponent;
class USceneComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class SMARTCITYSUITE_API AVehicle : public AActor
{
	GENERATED_BODY()

public:

	AVehicle(const FObjectInitializer& ObjectInitializer);

};