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

AGameMode_Main::AGameMode_Main() :
                                 Super()
{
}

void AGameMode_Main::OnConstruction(
	const FTransform& Transform
	)
{
	Super::OnConstruction(Transform);
}

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	const auto Time = FHttpModule::Get().GetHttpActivityTimeout();

	// 天气
	UWeatherSystem::GetInstance()->RegisterCallback();
	UWeatherSystem::GetInstance()->ResetTime();

	//
	TArray<AActor*> ResultAry;

	// 视角
	// SmartCityCommand::ReplyCameraTransform();

	// 
	USceneInteractionWorldSystem::GetInstance()->InitializeSceneActors();

#pragma region 场景
	// 整楼、外墙
	ProcessExternalWall();

	// 楼层
	ProcessFloor();

	// 区域
	ProcessSpace();
#pragma endregion

	FOnActorSpawned::FDelegate Delegate_OnActorSpawned;

	Delegate_OnActorSpawned.BindLambda(
	                                   [](
	                                   AActor* ActorPtr
	                                   )
	                                   {
		                                   if (ActorPtr && ActorPtr->IsA(AFloorHelper::StaticClass()))
		                                   {
			                                   PRINTINVOKEINFO();
		                                   }
	                                   }
	                                  );

	// GWorld->AddOnActorSpawnedHandler(Delegate_OnActorSpawned);

	FApp::SetBenchmarking(true);

	// GEngine->GetFirstLocalPlayerController(GetWorld())->DisableInput(nullptr);
	//  FSlateApplication::Get().SetAllUserFocusToGameViewport();

	auto Handle = UWeatherSystem::GetInstance()->GetDynamicSky()->OnHourChanged.AddCallback(
		 std::bind(
		           &ThisClass::OnHourChanged,
		           this,
		           std::placeholders::_1
		          )
		);
	Handle->bIsAutoUnregister = false;
	
	ExcuteCMD();
}

void AGameMode_Main::EndPlay(
	const EEndPlayReason::Type EndPlayReason
	)
{
	Super::EndPlay(EndPlayReason);
}

void AGameMode_Main::BeginDestroy()
{
	Super::BeginDestroy();
}

void AGameMode_Main::ProcessExternalWall()
{
}

void AGameMode_Main::ProcessSpace()
{
}

void AGameMode_Main::ExcuteCMD()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	// if (!PC) return;
	//
	// PC->ConsoleCommand(TEXT("sg.ShadowQuality 0"), true);
	// PC->ConsoleCommand(TEXT("sg.PostProcessQuality 0"), true);
	// PC->ConsoleCommand(TEXT("r.Lumen.GlobalIllumination 0"), true);
	// PC->ConsoleCommand(TEXT("r.Lumen.Reflections 0"), true);
	// PC->ConsoleCommand(TEXT("t.MaxFPS 30"), true);
}

void AGameMode_Main::ProcessFloor()
{
	TArray<AActor*> ResultAry;
	UGameplayStatics::GetAllActorsOfClass(
	                                      this,
	                                      AStaticMeshActor::StaticClass(),
	                                      ResultAry
	                                     );

	for (auto Iter : ResultAry)
	{
		auto STMPtr = Cast<AStaticMeshActor>(Iter);
		if (STMPtr)
		{
			auto STPtr = STMPtr->GetStaticMeshComponent()->GetStaticMesh();
			if (!STPtr)
			{
				continue;
			}
			auto AUDPtr = STPtr
				->
				GetAssetUserDataOfClass(
				                        UDatasmithAssetUserData::StaticClass()
				                       );
			if (AUDPtr)
			{
			}
		}
	}
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
