// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PlayerControllerBase.generated.h"

class UGTComponent;
class UPixelStreamingInput;
class UAsyncTaskInGameSubSysteam;
class UAsyncTaskInThreadSubSysteam;

/**
 *
 */
UCLASS()
class LAYERCONTROLLER_API APlayerControllerBase :
	public APlayerController
{
	GENERATED_BODY()

	friend UAsyncTaskInGameSubSysteam;
	friend UAsyncTaskInThreadSubSysteam;
	
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
