#include "SceneInteractionDecorator_Mode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "AssetRefMap.h"
#include "GameOptions.h"
#include "LogWriter.h"
#include "MessageBody.h"
#include "SceneInteractionWorldSystem.h"
#include "Dynamic_WeatherBase.h"
#include "FireMark.h"
#include "FloorHelper.h"
#include "SceneElement_PWR_Pipe.h"
#include "TemplateHelper.h"
#include "FloorHelperBase.h"
#include "SceneElementCategory.h"
#include "SceneElement_Space.h"
#include "SmartCitySuiteTags.h"
#include "WeatherSystem.h"
#include "SceneElementManagger_AccessControl.h"
#include "SceneInteractionDecorator_Area.h"

FEmpty_Decorator::FEmpty_Decorator() :
                                     Super(
                                          )
{
}

void FTour_Decorator::Entry()
{
	Super::Entry();
}

FTour_Decorator::FTour_Decorator() :
                                   Super(
                                        )
{
}

bool FTour_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	PRINTFUNCSTR();

	return Super::Operation(OperatorType);
}

FSceneMode_Decorator::FSceneMode_Decorator() :
                                             Super(
                                                  )
{
}

void FSceneMode_Decorator::Entry()
{
	Super::Entry();
}

bool FSceneMode_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	PRINTFUNCSTR();

	return Super::Operation(OperatorType);
}

FEmergencyMode_Decorator::FEmergencyMode_Decorator() :
                                                     Super(
                                                          )
{
}

void FEmergencyMode_Decorator::Entry()
{
	Super::Entry();

	auto AreaDecoratorSPtr =
		DynamicCastSharedPtr<FArea_Decorator>(
		                                      USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			                                       USmartCitySuiteTags::Interaction_Area
			                                      )
		                                     );

	if (!AreaDecoratorSPtr)
	{
		return;
	}

	Spawn(AreaDecoratorSPtr);
}

void FEmergencyMode_Decorator::Quit()
{
	Clear();
	Super::Quit();
}

void FEmergencyMode_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (NewDecoratorSPtr)
	{
		Clear();
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area))
		{
			if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor))
			{
				auto AreaDecoratorSPtr =
					DynamicCastSharedPtr<FArea_Decorator>(NewDecoratorSPtr);

				Spawn(AreaDecoratorSPtr);
			}
		}
	}
}

void FEmergencyMode_Decorator::OnOtherDecoratorQuit(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorQuit(NewDecoratorSPtr);
}

void FEmergencyMode_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

void FEmergencyMode_Decorator::Spawn(
	const TSharedPtr<FArea_Decorator>& AreaDecoratorSPtr
	)
{
	if (!AreaDecoratorSPtr)
	{
		return;
	}

	for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		const auto AreaTag = AreaDecoratorSPtr->GetCurrentInteraction_Area();
		if (Iter.Value->NavagationPaths.ToSoftObjectPath().IsValid())
		{
		}

		if (Iter.Value->FloorTag == AreaDecoratorSPtr->GetBranchDecoratorType())
		{
			if (Iter.Value->SceneElementCategoryMap.Contains(USmartCitySuiteTags::SceneElement_Category_Space))
			{
				auto FireMarkClass = UAssetRefMap::GetInstance()->FireMarkClass;
				auto WarningMarkClass = UAssetRefMap::GetInstance()->WarningMarkClass;

				const auto Spaces = Iter.Value->SceneElementCategoryMap[
					USmartCitySuiteTags::SceneElement_Category_Space];
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				const auto FloorBox = Iter.Value->BoxComponentPtr->GetScaledBoxExtent();
				const auto FloorPt = Iter.Value->BoxComponentPtr->GetComponentLocation();

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElement_Space>(ActorIter);
					if (SceneElementBasePtr)
					{
						for (auto SpaceBoxIter : SceneElementBasePtr->CollisionComponentsAry)
						{
							if (FMath::RandRange(0, 100) > 60)
							{
								auto FireMarkPtr = GetWorldImp()->SpawnActor<AFireMark>(
									 FireMarkClass,
									 SpaceBoxIter->GetComponentLocation(),
									 FRotator::ZeroRotator
									);
								FireMarkSet.Add(FireMarkPtr);
								if (FireMarkSet.Num() > 3)
								{
									return;
								}
							}
							else if (FMath::RandRange(0, 100) > 60)
							{
								auto FireMarkPtr = GetWorldImp()->SpawnActor<AWarningMark>(
									 WarningMarkClass,
									 FVector(
									         SpaceBoxIter->GetComponentLocation().X,
									         SpaceBoxIter->GetComponentLocation().Y,
									         FloorPt.Z + FloorBox.Z
									        ),
									 FRotator::ZeroRotator
									);
								WarningMarkSet.Add(FireMarkPtr);
								if (WarningMarkSet.Num() > 3)
								{
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void FEmergencyMode_Decorator::Clear()
{
}

FEnvironmentalPerceptionMode_Decorator::FEnvironmentalPerceptionMode_Decorator() :
	Super(
	     )
{
}

FSafeManagementMode_Decorator::FSafeManagementMode_Decorator() :
                                                               Super(
                                                                    )
{
}

FRadarMode_Decorator::FRadarMode_Decorator() :
                                             Super(
                                                  )
{
}

FRadarMode_Decorator::~FRadarMode_Decorator()
{
}

void FRadarMode_Decorator::Entry()
{
	Super::Entry();
}

void FRadarMode_Decorator::Quit()
{
	Super::Quit();
}

bool FRadarMode_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	return Super::Operation(OperatorType);
}

FDeviceManaggerMode_Decorator::FDeviceManaggerMode_Decorator() :
                                                               Super(
                                                                    )
{
}

FDeviceManaggerMode_Decorator::FDeviceManaggerMode_Decorator(
	FGameplayTag InBranchDecoratorType
	) :
	  Super(
	       )
{
}

FDeviceManaggerPWRMode_Decorator::FDeviceManaggerPWRMode_Decorator(
	) :
	  Super(
	        USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR
	       )
{
}

FDeviceManaggerPWRMode_Decorator::FDeviceManaggerPWRMode_Decorator(
	FGameplayTag InBranchDecoratorType
	) :
	  Super(
	        InBranchDecoratorType
	       )
{
}

FEnergyMode_Decorator::FEnergyMode_Decorator() :
                                               Super(
                                                    )
{
}

void FEnergyMode_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

FHVACMode_Decorator::FHVACMode_Decorator() :
                                           Super(
                                                 USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_HVAC
                                                )
{
}

FLightingMode_Decorator::FLightingMode_Decorator() :
                                                   Super(
                                                         USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
                                                        )
{
}

void FLightingMode_Decorator::Entry()
{
	Super::Entry();
}

void FLightingMode_Decorator::Quit()
{
	Super::Quit();
}

void FLightingMode_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);
}

void FLightingMode_Decorator::OnOtherDecoratorQuit(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorQuit(NewDecoratorSPtr);
}

FAccessControlMode_Decorator::FAccessControlMode_Decorator() :
                                                             Super(
                                                                   USmartCitySuiteTags::Interaction_Mode_DeviceManagger_ELV_AccessControl
                                                                  )
{
}

void FAccessControlMode_Decorator::Entry()
{
	Super::Entry();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorldImp(), ASceneElementManagger_AccessControl::StaticClass(), OutActors);

	USceneInteractionWorldSystem::GetInstance()->ClearFocus();
	USceneInteractionWorldSystem::GetInstance()->ClearRouteMarker();

	FSceneElementConditional SceneActorConditional;

	SceneActorConditional.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Mode);

	TSet<ASceneElementBase*> FocusActorsAry;

	for (auto Iter : OutActors)
	{
		auto SceneElementActorPtr = Cast<ASceneElementBase>(Iter);
		if (SceneElementActorPtr)
		{
			FocusActorsAry.Add(SceneElementActorPtr);
		}
	}
	USceneInteractionWorldSystem::GetInstance()->UpdateInteractionType(FocusActorsAry, SceneActorConditional);
}

FElevatorMode_Decorator::FElevatorMode_Decorator() :
                                                   Super(
                                                        )
{
}

void FElevatorMode_Decorator::Entry()
{
	Super::Entry();

	UAssetRefMap::GetInstance()->PostProcessVolume.LoadSynchronous()->bEnabled = false;
	UAssetRefMap::GetInstance()->PostProcessVolume_Elevator.LoadSynchronous()->bEnabled = true;
	UAssetRefMap::GetInstance()->PostProcessVolume_TemperatureMap.LoadSynchronous()->bEnabled = false;
}

void FElevatorMode_Decorator::Quit()
{
	UAssetRefMap::GetInstance()->PostProcessVolume.LoadSynchronous()->bEnabled = true;
	UAssetRefMap::GetInstance()->PostProcessVolume_Elevator.LoadSynchronous()->bEnabled = false;
	UAssetRefMap::GetInstance()->PostProcessVolume_TemperatureMap.LoadSynchronous()->bEnabled = false;

	Super::Quit();
}

void FElevatorMode_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

FSunShadeMode_Decorator::FSunShadeMode_Decorator() :
                                                   Super(
                                                         USmartCitySuiteTags::Interaction_Mode_DeviceManagger_SunShadow
                                                        )
{
}

FInteraction_Decorator::FInteraction_Decorator() :
                                                 Super(
                                                      )
{
	ControllConfig.CurrentWeather = WeatherSettings::Clear_Skies;

	ControllConfig.CurrentHour = 12;
}

void FInteraction_Decorator::Entry()
{
	Super::Entry();
}

FSingleDeviceMode_Decorator::FSingleDeviceMode_Decorator(
	) :
	  Super(
	       )

{
}

void FSingleDeviceMode_Decorator::Entry()
{
	Super::Entry();

}

FBatchControlMode_Decorator::FBatchControlMode_Decorator() :
                                                           Super(
                                                                )
{
}

void FBatchControlMode_Decorator::Entry()
{
	Super::Entry();

	Initial();
}

void FBatchControlMode_Decorator::ReEntry()
{
	Super::ReEntry();

	Initial();
}

void FBatchControlMode_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);
}

void FBatchControlMode_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);

	Process();
}

void FBatchControlMode_Decorator::Initial()
{
}

void FBatchControlMode_Decorator::Process()
{
	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_BatchControlDevice(
			 SceneActorConditional,
			 true,
			 false,
			 MulticastDelegate,
			 SceneElementSet,
			 FloorTag
			);
	};
}

FTemperatureMapMode_Decorator::FTemperatureMapMode_Decorator()
{
}

void FTemperatureMapMode_Decorator::Entry()
{
	FDecoratorBase::Entry();

	UAssetRefMap::GetInstance()->PostProcessVolume.LoadSynchronous()->bEnabled = false;
	UAssetRefMap::GetInstance()->PostProcessVolume_Elevator.LoadSynchronous()->bEnabled = false;
	UAssetRefMap::GetInstance()->PostProcessVolume_TemperatureMap.LoadSynchronous()->bEnabled = false;

	FDateTime Time(1, 1, 1, 12);

	UWeatherSystem::GetInstance()->AdjustTime(Time);
}

void FTemperatureMapMode_Decorator::Quit()
{
	UAssetRefMap::GetInstance()->PostProcessVolume.LoadSynchronous()->bEnabled = true;
	UAssetRefMap::GetInstance()->PostProcessVolume_Elevator.LoadSynchronous()->bEnabled = false;
	UAssetRefMap::GetInstance()->PostProcessVolume_TemperatureMap.LoadSynchronous()->bEnabled = false;

	auto DecoratorSPtr =
		DynamicCastSharedPtr<FInteraction_Decorator>(
		                                             USceneInteractionWorldSystem::GetInstance()->
		                                             GetDecorator(
		                                                          USmartCitySuiteTags::Interaction_Interaction
		                                                         )
		                                            );

	if (!DecoratorSPtr)
	{
		return;
	}

	const auto ConfigControlConfig = DecoratorSPtr->GetConfigControlConfig();

	FDateTime Time(1, 1, 1, ConfigControlConfig.CurrentHour);

	UWeatherSystem::GetInstance()->AdjustTime(Time);


	FDecoratorBase::Quit();
}

void FTemperatureMapMode_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (NewDecoratorSPtr)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area))
		{
			if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor))
			{
				Clear();
			}
		}
	}
}

FGuid FTemperatureMapMode_Decorator::AddPoint(
	const FVector& Pt,
	float Distance,
	float Value
	)
{
	Distance = FMath::Clamp(Distance, 0.f, 1000.f);
	Value = FMath::Clamp(Value, 0.f, 1000.f);

	for (int32 Index = 0; Index < UAssetRefMap::GetInstance()->TemperatureMapMaxCount; Index++)
	{
		if (PtsMap.Contains(Index))
		{
		}
		else
		{
			auto Result = FGuid::NewGuid();

			PtsMap.Add(Index, Result);

			auto MCPtr = UAssetRefMap::GetInstance()->TemperatureMapMCRef.LoadSynchronous();
			auto MPCI = GetWorldImp()->GetParameterCollectionInstance(MCPtr);

			FVector4 Vec4(Pt);

			Vec4.W = (Distance * 10000) + Value;

			if (Index <= 0)
			{
				MPCI->SetVectorParameterValue(TEXT("Vector"), Vec4);
			}
			else
			{
				MPCI->SetVectorParameterValue(*FString::Printf(TEXT("Vector%d"), Index), Vec4);
			}

			return Result;
		}
	}

	return FGuid();
}

void FTemperatureMapMode_Decorator::UpdatePoint(
	const FGuid& ID,
	const FVector& Pt,
	float Distance,
	float Value
	)
{
	Distance = FMath::Clamp(Distance, 500.f, 1000.f);
	Value = FMath::Clamp(Value, 800.f, 1000.f);

	for (const auto& Iter : PtsMap)
	{
		if (Iter.Value == ID)
		{
			auto MCPtr = UAssetRefMap::GetInstance()->TemperatureMapMCRef.LoadSynchronous();
			auto MPCI = GetWorldImp()->GetParameterCollectionInstance(MCPtr);

			FVector4 Vec4(Pt);

			Vec4.W = (Distance * 10000) + Value;

			if (Iter.Key <= 0)
			{
				MPCI->SetVectorParameterValue(TEXT("Vector"), Vec4);
			}
			else
			{
				MPCI->SetVectorParameterValue(*FString::Printf(TEXT("Vector%d"), Iter.Key), Vec4);
			}
		}
	}
}

void FTemperatureMapMode_Decorator::Remove(
	const FGuid& ID
	)
{
	for (auto Iter = PtsMap.CreateIterator(); Iter; ++Iter)
	{
		if (Iter->Value == ID)
		{
			auto MCPtr = UAssetRefMap::GetInstance()->TemperatureMapMCRef.LoadSynchronous();
			auto MPCI = GetWorldImp()->GetParameterCollectionInstance(MCPtr);

			if (Iter->Key <= 0)
			{
				MPCI->SetVectorParameterValue(TEXT("Vector"), FColor(0, 0, 0, 0));
			}
			else
			{
				MPCI->SetVectorParameterValue(*FString::Printf(TEXT("Vector%d"), Iter->Key), FColor(0, 0, 0, 0));
			}

			Iter.RemoveCurrent();
		}
	}
}

void FTemperatureMapMode_Decorator::Clear()
{
	auto MCPtr = UAssetRefMap::GetInstance()->TemperatureMapMCRef.LoadSynchronous();
	auto MPCI = GetWorldImp()->GetParameterCollectionInstance(MCPtr);

	for (int32 Index = 0; Index < UAssetRefMap::GetInstance()->TemperatureMapMaxCount; Index++)
	{
		if (Index <= 0)
		{
			MPCI->SetVectorParameterValue(TEXT("Vector"), FColor(0, 0, 0, 0));
		}
		else
		{
			MPCI->SetVectorParameterValue(*FString::Printf(TEXT("Vector%d"), Index), FColor(0, 0, 0, 0));
		}
	}
}
