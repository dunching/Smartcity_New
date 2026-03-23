// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HumanCharacter_AI.h"
#include "PlanetControllerInterface.h"
#include "PlayerControllerBase.h"

#include "PlanetPlayerController.generated.h"

class UPlayerControllerGameplayTasksComponent;
class UPixelStreamingInput;

/**
 *
 */
UCLASS()
class SMARTCITY_API APlanetPlayerController :
	public APlayerControllerBase,
	public IPlanetControllerInterface
{
	GENERATED_BODY()

public:

	APlanetPlayerController(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void OnPossess(
		APawn* InPawn
		) override;

};
