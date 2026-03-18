// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "SceneElementTools.generated.h"

class UDatasmithAssetUserData;

UCLASS()
class BUILDINGTOOLS_API USceneElementTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool CheckIsJiaCeng(
		UDatasmithAssetUserData* AUDPtr
		);
};
