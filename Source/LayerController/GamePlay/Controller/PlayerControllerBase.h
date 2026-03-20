// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PlayerControllerBase.generated.h"

class UGTComponent;
class UPixelStreamingInput;

/**
 *
 */
UCLASS()
class LAYERCONTROLLER_API APlayerControllerBase :
	public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControllerBase(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void OnPossess(
		APawn* InPawn
		) override;

	UPROPERTY()
	TObjectPtr<UGTComponent> GameplayTasksComponentPtr = nullptr;

	UPROPERTY()
	TObjectPtr<UPixelStreamingInput> PixelStreamingInputPtr = nullptr;

};
