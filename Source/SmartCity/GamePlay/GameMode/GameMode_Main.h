// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "GameMode_Main.generated.h"

extern SMARTCITY_API class AShiYuGameMode* GGameModePtr;

class UInventoryComponent;
struct FCharacterProxy;

/**
 *
 */
UCLASS()
class SMARTCITY_API AGameMode_Main : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay()override;

private:

	void OnHourChanged(int32 Hour);
	
};
