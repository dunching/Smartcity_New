// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PlayerControllerBase.generated.h"

class UGTComponent;
class UPixelStreamingInput;
class UAsyncTaskSubSysteam;

/**
 *
 */
UCLASS()
class LAYERCONTROLLER_API APlayerControllerBase :
	public APlayerController
{
	GENERATED_BODY()

	friend UAsyncTaskSubSysteam;
	
public:

	APlayerControllerBase(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void OnPossess(
		APawn* InPawn
		) override;

	UPROPERTY()
	TObjectPtr<UPixelStreamingInput> PixelStreamingInputPtr = nullptr;

protected:
	
	UPROPERTY()
	TObjectPtr<UGTComponent> GameplayTasksComponentPtr = nullptr;

};
