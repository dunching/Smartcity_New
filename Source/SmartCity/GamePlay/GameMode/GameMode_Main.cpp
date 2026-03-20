// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameMode_Main.h"

#include "AssetRefMap.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "GameplayCommand.h"
#include "GameplayTagsLibrary.h"
#include "SceneInteractionWorldSystem.h"
#include "WeatherSystem.h"
#include "DatasmithAssetUserData.h"
#include "Dynamic_SkyBase.h"
#include "FloorHelper.h"
#include "HttpModule.h"
#include "IPSSI.h"
#include "LogWriter.h"
#include "ViewTowerProcessor.h"
#include "TourPawn.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	// 天气
	UWeatherSystem::GetInstance()->RegisterCallback();
	UWeatherSystem::GetInstance()->ResetTime();

	auto Handle = UWeatherSystem::GetInstance()->GetDynamicSky()->OnHourChanged.AddCallback(
		 std::bind(
		           &ThisClass::OnHourChanged,
		           this,
		           std::placeholders::_1
		          )
		);
	Handle->bIsAutoUnregister = false;

	USceneInteractionWorldSystem::GetInstance()->InitializeSceneActors();
}

void AGameMode_Main::OnHourChanged(
	int32 Hour
	)
{
	if (Hour > 18 || Hour < 8)
	{
		auto SmartCityMCPtr = UAssetRefMap::GetInstance()->SmartCityMCRef.LoadSynchronous();
		if (SmartCityMCPtr)
		{
			auto MPCI = GetWorld()->GetParameterCollectionInstance(SmartCityMCPtr);
			MPCI->SetScalarParameterValue(TEXT("自发光"), 0.1);
		}
		return;
	}

	auto SmartCityMCPtr = UAssetRefMap::GetInstance()->SmartCityMCRef.LoadSynchronous();
	if (SmartCityMCPtr)
	{
		auto MPCI = GetWorld()->GetParameterCollectionInstance(SmartCityMCPtr);
		MPCI->SetScalarParameterValue(TEXT("自发光"), 0.f);
	}
}
