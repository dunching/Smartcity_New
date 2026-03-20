#include "FloorHelper.h"

#include "Components/BoxComponent.h"
#include "Components/RectLightComponent.h"
#include "UObject/SoftObjectPtr.h"

#include "AssetRefMap.h"
#include "CollisionDataStruct.h"
#include "ComputerMark.h"
#include "Dynamic_SkyBase.h"
#include "FloorHelper_Description.h"
#include "SceneElementCategory.h"
#include "SceneElement_Computer.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"
#include "ViewerPawnBase.h"
#include "WeatherSystem.h"

AFloorHelper::AFloorHelper(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	FloorHelper_DescriptionAttachTransform = CreateDefaultSubobject<USceneComponent>(
		 TEXT("FloorHelper_DescriptionAttachTransform")
		);
	FloorHelper_DescriptionAttachTransform->SetMobility(EComponentMobility::Movable);

	FloorHelper_DescriptionAttachTransform->SetupAttachment(BoxComponentPtr);

	FloorTMCPtr = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorTMC"));
	FloorTMCPtr->SetupAttachment(BoxComponentPtr);

	FloorTMCPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FloorTMCPtr->SetCollisionObjectType(Floor_Object);
	FloorTMCPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	TemporatureNapTMCPtr = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TemporatureNapTMC"));
	TemporatureNapTMCPtr->SetupAttachment(BoxComponentPtr);

	TemporatureNapTMCPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TemporatureNapTMCPtr->SetCollisionObjectType(Floor_Object);
	TemporatureNapTMCPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFloorHelper::BeginPlay()
{
	Super::BeginPlay();

	auto Handle = UWeatherSystem::GetInstance()->GetDynamicSky()->OnHourChanged.AddCallback(
		 std::bind(
		           &ThisClass::OnHourChanged,
		           this,
		           std::placeholders::_1
		          )
		);
	Handle->bIsAutoUnregister = false;

	const auto Box = BoxComponentPtr->GetLocalBounds();

	if (UAssetRefMap::GetInstance()->FloorSTM.ToSoftObjectPath().IsValid())
	{
		FloorTMCPtr->SetStaticMesh(UAssetRefMap::GetInstance()->FloorSTM.LoadSynchronous());
	}

	if (UAssetRefMap::GetInstance()->ViewElevatorMatInst.ToSoftObjectPath().IsValid())
	{
		FloorTMCPtr->SetMaterial(0, UAssetRefMap::GetInstance()->ViewElevatorMatInst.LoadSynchronous());
	}

	{
		FBox TempBox;
		FloorTMCPtr->GetLocalBounds(TempBox.Min, TempBox.Max);

		FloorTMCPtr->SetRelativeLocation(FVector(0, 0, -Box.BoxExtent.Z + (TempBox.GetSize().Z / 2 * .5f)));
		FloorTMCPtr->SetRelativeRotation(FRotator(0, 0, 0));
		FloorTMCPtr->SetRelativeScale3D(
		                                FVector(
		                                        Box.GetBox().GetSize().X / TempBox.GetSize().X,
		                                        Box.GetBox().GetSize().Y / TempBox.GetSize().Y,
		                                        .5f
		                                       )
		                               );
	}
	{
		TemporatureNapTMCPtr->SetStaticMesh(UAssetRefMap::GetInstance()->TemporatureSTM.LoadSynchronous());
		TemporatureNapTMCPtr->SetMaterial(0, UAssetRefMap::GetInstance()->ViewTempporatureMapMatInst.LoadSynchronous());

		FBox TempBox;
		TemporatureNapTMCPtr->GetLocalBounds(TempBox.Min, TempBox.Max);

		TemporatureNapTMCPtr->SetRelativeLocation(
		                                          FVector(
		                                                  0,
		                                                  0,
		                                                  Box.BoxExtent.Z - (TempBox.GetSize().Z / 2 * .5f) - 50.f
		                                                 )
		                                         );
		TemporatureNapTMCPtr->SetRelativeRotation(FRotator(0, 0, 0));
		TemporatureNapTMCPtr->SetRelativeScale3D(
		                                         FVector(
		                                                 Box.GetBox().GetSize().X / TempBox.GetSize().X,
		                                                 Box.GetBox().GetSize().Y / TempBox.GetSize().Y,
		                                                 .5f
		                                                )
		                                        );
	}
}

void AFloorHelper::OnConstruction(
	const FTransform& Transform
	)
{
	Super::OnConstruction(Transform);

	const auto Box = BoxComponentPtr->GetLocalBounds();

	FloorHelper_DescriptionAttachTransform->SetRelativeLocation(FVector(-Box.BoxExtent.X, -Box.BoxExtent.Y, 0));
	FloorHelper_DescriptionAttachTransform->SetRelativeRotation(FRotator(0, 180, 0));
}

void AFloorHelper::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) &&
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_ViewElevator)
		)
		{
			OnExternalWall();

			if (FloorHelper_DescriptionPtr)
			{
				FloorHelper_DescriptionPtr->Destroy();
			}

			FloorHelper_DescriptionPtr = nullptr;

			if (bIsSup)
			{
				FloorTMCPtr->SetHiddenInGame(true);
			}
			else
			{
				FloorTMCPtr->SetHiddenInGame(false);
			}
			TemporatureNapTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall)
		)
		{
			OnExternalWall();
			if (FloorHelper_DescriptionPtr)
			{
			}
			else
			{
				if (bDisplayDescritpion)
				{
					FActorSpawnParameters SpawnParameters;

					SpawnParameters.CustomPreSpawnInitalization = [this](
						auto ActorPtr
						)
						{
							auto FloorPtr = Cast<AFloorHelper_Description>(ActorPtr);
							if (FloorPtr)
							{
								FloorPtr->SetFloor(this);
							}
						};
					FloorHelper_DescriptionPtr = GetWorld()->SpawnActor<AFloorHelper_Description>(
						 UAssetRefMap::GetInstance()->FloorHelper_DescriptionClass,
						 SpawnParameters
						);
				}
			}

			FloorTMCPtr->SetHiddenInGame(true);
			TemporatureNapTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			if (FloorHelper_DescriptionPtr)
			{
				FloorHelper_DescriptionPtr->Destroy();
			}

			FloorHelper_DescriptionPtr = nullptr;

			FloorTMCPtr->SetHiddenInGame(true);
			TemporatureNapTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) &&
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_TemperatureMap)))
		{
			
			
			if (bIsSup)
			{
				TemporatureNapTMCPtr->SetHiddenInGame(true);
			}
			else
			{
				TemporatureNapTMCPtr->SetHiddenInGame(false);
			}
			FloorTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			if (FloorHelper_DescriptionPtr)
			{
				FloorHelper_DescriptionPtr->Destroy();
			}

			FloorHelper_DescriptionPtr = nullptr;

			FloorTMCPtr->SetHiddenInGame(true);
			TemporatureNapTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_Focus)
		)
		{
			if (FloorHelper_DescriptionPtr)
			{
				FloorHelper_DescriptionPtr->Destroy();
			}
			FloorHelper_DescriptionPtr = nullptr;

			FloorTMCPtr->SetHiddenInGame(true);
			TemporatureNapTMCPtr->SetHiddenInGame(true);

			return;
		}
	}
	{
		if (ConditionalSet.ConditionalSet.IsEmpty())
		{
		}

		if (FloorHelper_DescriptionPtr)
		{
			FloorHelper_DescriptionPtr->Destroy();
		}
		FloorHelper_DescriptionPtr = nullptr;

		FloorTMCPtr->SetHiddenInGame(true);
		TemporatureNapTMCPtr->SetHiddenInGame(true);

		return;
	}
}

void AFloorHelper_Computer::OnConstruction(
	const FTransform& Transform
	)
{
	Super::OnConstruction(Transform);
}

void AFloorHelper_Computer::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall)
		)
		{
			SetActorHiddenInGame(true);

			if (ComputerMarkRef.ToSoftObjectPath().IsValid())
			{
				ComputerMarkRef.LoadSynchronous()->SetActorHiddenInGame(true);
			}

			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			SetActorHiddenInGame(true);

			if (ComputerMarkRef.ToSoftObjectPath().IsValid())
			{
				ComputerMarkRef.LoadSynchronous()->SetActorHiddenInGame(true);
			}

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF) ||
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			SetActorHiddenInGame(false);

			if (ComputerMarkRef.ToSoftObjectPath().IsValid())
			{
				ComputerMarkRef.LoadSynchronous()->SetActorHiddenInGame(false);
			}

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			SetActorHiddenInGame(false);

			if (ComputerMarkRef.ToSoftObjectPath().IsValid())
			{
				ComputerMarkRef.LoadSynchronous()->SetActorHiddenInGame(true);
			}

			return;
		}
	}
	{
		if (ComputerMarkRef.ToSoftObjectPath().IsValid())
		{
			ComputerMarkRef.LoadSynchronous()->SetActorHiddenInGame(true);
		}

		return;
	}
}

TMap<FString, TSoftObjectPtr<AViewerPawnBase>> AFloorHelper_Computer::GetPresetBuildingCameraSeat() const
{
	auto Result = Super::GetPresetBuildingCameraSeat();

	for (const auto& Iter : SceneElementCategoryMap)
	{
		TArray<AActor*> OutActors;

		Iter.Value->GetAttachedActors(OutActors, true, true);

		for (auto ActorIter : OutActors)
		{
			auto SceneElementBasePtr = Cast<ASceneElement_Computer>(ActorIter);
			if (SceneElementBasePtr)
			{
				Result.Add(SceneElementBasePtr->DeviceTypeStr, nullptr);
			}
		}
	}

	return Result;
}

ABuilding_Floor_Mask::ABuilding_Floor_Mask(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ABuilding_Floor_Mask::SetFloor(
	AFloorHelper* FloorPtr
	)
{
	if (FloorPtr)
	{
		FVector Origin;
		FVector BoxExtent;

		FloorPtr->GetActorBounds(false, Origin, BoxExtent);

		SetActorLocation(Origin - FVector(0, 0, BoxExtent.Z));
	}
}

void AFloorHelper::OnHourChanged(
	int32 Hour
	)
{
	if (Hour > 18 || Hour < 8)
	{
		for (auto Iter : RectLightComponentAry)
		{
			// Iter->SetHiddenInGame(false	);
		}
		return;
	}
	for (auto Iter : RectLightComponentAry)
	{
		Iter->SetHiddenInGame(true);
	}
}

void AFloorHelper::OnExternalWall()
{
	const auto Hour = UWeatherSystem::GetInstance()->GetDynamicSky()->GetCurrentHour();

	if (Hour > 18 || Hour < 8)
	{
		for (auto Iter : RectLightComponentAry)
		{
			// Iter->SetHiddenInGame(false	);
		}
		return;
	}
	for (auto Iter : RectLightComponentAry)
	{
		Iter->SetHiddenInGame(true);
	}
}
