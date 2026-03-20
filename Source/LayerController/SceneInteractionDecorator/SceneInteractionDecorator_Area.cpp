#include "SceneInteractionDecorator_Area.h"

#include "AssetRefMap.h"
#include "GameOptions.h"
#include "LogWriter.h"
#include "SceneInteractionWorldSystem.h"
#include "Building_Floor.h"
#include "Dynamic_WeatherBase.h"
#include "FloorHelper.h"
#include "TemplateHelper.h"
#include "FloorHelperBase.h"
#include "IPSSI.h"
#include "SmartCitySuiteTags.h"
#include "ViewSingleFloorProcessor.h"
#include "WeatherSystem.h"
#include "ViewTowerProcessor.h"
#include "ViewSingleDeviceProcessor.h"
#include "ViewSingleFloorViewEnergyProcessor.h"
#include "ViewSplitFloorProcessor.h"
#include "SceneElement_Computer.h"
#include "SceneInteractionDecorator_Mode.h"
#include "ViewSingleSpaceProcessor.h"
#include "ViewSpecialAreaProcessor.h"
#include "ViewerPawnBase.h"
#include "ViewPeripheryProcessor.h"

FArea_Decorator::FArea_Decorator(
	) :
	  Super(
	       )
{
}

void FArea_Decorator::Entry()
{
	Super::Entry();
}

void FArea_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);
}

void FArea_Decorator::OnOtherDecoratorQuit(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorQuit(NewDecoratorSPtr);
}

void FArea_Decorator::InitialType(
	FGameplayTag InMainDecoratorType,
	FGameplayTag InBranchDecoratorType
	)
{
	Super::InitialType(InMainDecoratorType, InBranchDecoratorType);

	CurrentInteraction_Area = InBranchDecoratorType;
}

FGameplayTag FArea_Decorator::GetCurrentInteraction_Area() const
{
	return CurrentInteraction_Area;
}

void FArea_Decorator::UpdateDisplay()
{
}

void FArea_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
}

FViewTower_Decorator::FViewTower_Decorator(
	) :
	  Super(
	       )
{
}

void FViewTower_Decorator::Entry()
{
	Super::Entry();

	TSoftObjectPtr<AViewerPawnBase> DefaultBuildingCameraSeat;

	ON_SCOPE_EXIT
	{
		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<TourProcessor::FViewTowerProcessor>(
			 [DefaultBuildingCameraSeat](
			 auto NewProcessor
			 )
			 {
				 NewProcessor->DefaultBuildingCameraSeat = DefaultBuildingCameraSeat;
			 }
			);
	};

	{
		auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			 USmartCitySuiteTags::Interaction_Mode
			);
		if (
			DecoratorSPtr
		)
		{
			if (
				DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
			)
			{
				if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,
						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
				                                                            USmartCitySuiteTags::Interaction_Mode_EmergencySystem
				                                                           ))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,
						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
				                                                            USmartCitySuiteTags::Interaction_Mode_View
				                                                           ))
				{
					USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(FGameplayTag::EmptyTag);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().
				                        MatchesTag(USmartCitySuiteTags::Interaction_Mode_ViewElevator))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
					SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,
						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);


					DefaultBuildingCameraSeat = UAssetRefMap::GetInstance()->ViewElevatorCameraSeat.LoadSynchronous();
					return;
				}
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,

						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);
					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
			}
		}
		else
		{
		}
	}
	FSceneElementConditional SceneActorConditional;

	SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
	                                                                SceneActorConditional,
	                                                                true,
	                                                                true,
	                                                                MulticastDelegate
	                                                               );
}

void FViewTower_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().
		                           MatchesTag(USmartCitySuiteTags::Interaction_Mode_EmergencySystem))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().
		                           MatchesTag(USmartCitySuiteTags::Interaction_Mode_View))
		{
			// USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(FGameplayTag::EmptyTag);

			// return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                               USmartCitySuiteTags::Interaction_Mode_ViewElevator
		                                                              ))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );

			AdjustViewElevatorCamera();
			return;
		}
		else
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());


			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
	}
	else
	{
	}

	FSceneElementConditional SceneActorConditional;

	SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
	                                                                SceneActorConditional,
	                                                                true,
	                                                                false,
	                                                                MulticastDelegate
	                                                               );
}

bool FViewTower_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	Super::Operation(OperatorType);

	PRINTFUNCSTR();

	switch (OperatorType)
	{
	case EOperatorType::kLeftMouseButton:
	case EOperatorType::kRightMouseButton:
		{
		}
		break;
	case EOperatorType::kNone:
		break;
	default: ;
	}

	return false;
}

void FViewTower_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

void FViewTower_Decorator::AdjustViewElevatorCamera() const
{
}

FViewPeriphery_Decorator::FViewPeriphery_Decorator() :
                                                     Super(
                                                          )
{
}

void FViewPeriphery_Decorator::Entry()
{
	Super::Entry();

	ON_SCOPE_EXIT
	{
		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<TourProcessor::FViewPeripheryProcessor>(
			 [](
			 auto NewProcessor
			 )
			 {
			 }
			);
	};

	{
		auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			 USmartCitySuiteTags::Interaction_Mode
			);
		if (
			DecoratorSPtr
		)
		{
			if (
				DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
			)
			{
				if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,

						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
				                                                            USmartCitySuiteTags::Interaction_Mode_EmergencySystem
				                                                           ))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,

						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
				                                                            USmartCitySuiteTags::Interaction_Mode_View
				                                                           ))
				{
					USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(FGameplayTag::EmptyTag);

					return;
				}
				else if (DecoratorSPtr->GetBranchDecoratorType().
				                        MatchesTag(USmartCitySuiteTags::Interaction_Mode_ViewElevator))
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
					SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,

						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
				{
					FSceneElementConditional SceneActorConditional;

					SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

					TMulticastDelegate<void(
						bool,

						UGT_SwitchSceneElement_Base*


						
						)> MulticastDelegate;

					MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);
					MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

					USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
						 SceneActorConditional,
						 true,
						 true,
						 MulticastDelegate
						);

					return;
				}
			}
		}
		else
		{
		}
	}
	FSceneElementConditional SceneActorConditional;

	SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
	                                                                SceneActorConditional,
	                                                                true,
	                                                                true,
	                                                                MulticastDelegate
	                                                               );
}

void FViewPeriphery_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().
		                           MatchesTag(USmartCitySuiteTags::Interaction_Mode_EmergencySystem))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().
		                           MatchesTag(USmartCitySuiteTags::Interaction_Mode_View))
		{
			// USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(FGameplayTag::EmptyTag);

			// return;
		}
		else if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                               USmartCitySuiteTags::Interaction_Mode_ViewElevator
		                                                              ))
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
		else
		{
			FSceneElementConditional SceneActorConditional;

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());


			TMulticastDelegate<void(
				bool,

				UGT_SwitchSceneElement_Base*


				
				)> MulticastDelegate;

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
			                                                                SceneActorConditional,
			                                                                true,
			                                                                false,
			                                                                MulticastDelegate
			                                                               );
			return;
		}
	}
	else
	{
	}

	FSceneElementConditional SceneActorConditional;

	SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Tower(
	                                                                SceneActorConditional,
	                                                                true,
	                                                                false,
	                                                                MulticastDelegate
	                                                               );
}

bool FViewPeriphery_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	Super::Operation(OperatorType);

	PRINTFUNCSTR();

	switch (OperatorType)
	{
	case EOperatorType::kLeftMouseButton:
	case EOperatorType::kRightMouseButton:
		{
		}
		break;
	case EOperatorType::kNone:
		break;
	default: ;
	}

	return false;
}

void FViewPeriphery_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

FSplitFloor_Decorator::FSplitFloor_Decorator(
	) :
	  Super(
	       )
{
}

void FSplitFloor_Decorator::Entry()
{
	Super::Entry();

	UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<TourProcessor::FViewSplitFloorProcessor>(
		 [](
		 auto NewProcessor
		 )
		 {
		 }
		);
}

void FSplitFloor_Decorator::Quit()
{
	Super::Quit();
}

void FSplitFloor_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);
}

bool FSplitFloor_Decorator::NeedAsync() const
{
	return true;
}

bool FSplitFloor_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	return Super::Operation(OperatorType);
}

FFloor_Decorator::FFloor_Decorator(
	) :
	  Super(
	       )
{
}

FFloor_Decorator::~FFloor_Decorator()
{
}

void FFloor_Decorator::Entry()
{
	Super::Entry();

	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateWeather(WeatherSettings::Clear_Skies);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverageMunualOverride(true);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverage(0);

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	bool bUseTemporaComfig = false;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 30;
			                                                                  Config.PillarTranlucent = 30;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	TSet<TObjectPtr<ASceneElementBase>> SceneElementSet;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();
		AdjustCamera();
	};

	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	ON_SCOPE_EXIT
	{
		TSet<FGameplayTag> FloorSet = GetFloorSet();

		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Floor(
		                                                                SceneActorConditional,
		                                                                true,
		                                                                true,
		                                                                FloorSet,
		                                                                MulticastDelegate,
		                                                                SceneElementSet
		                                                               );
	};

	auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
		 USmartCitySuiteTags::Interaction_Mode
		);

	if (
		DecoratorSPtr
	)
	{
		if (
			DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_BatchControl))
			{
				USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(
					 USmartCitySuiteTags::Interaction_Mode_Empty
					);
			}
		}
	}

	if (
		DecoratorSPtr
	)
	{
		if (
			DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().
			                   MatchesTag(USmartCitySuiteTags::Interaction_Mode_ViewElevator))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EnergyManagement
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorViewEnergyProcessor>(
					                                                    [this](
					                                                    auto NewProcessor
					                                                    )
					                                                    {
						                                                    NewProcessor->Interaction_Area =
							                                                    GetBranchDecoratorType();
					                                                    }
					                                                   );

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_BatchControl
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
				// MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_TemperatureMap
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
				// MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				return;
			}
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);
				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
					TourProcessor::FViewSingleFloorProcessor>(
					                                          [this](
					                                          auto NewProcessor
					                                          )
					                                          {
						                                          NewProcessor->Interaction_Area =
							                                          GetBranchDecoratorType();
					                                          }
					                                         );

				return;
			}
		}
	}

	SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
		TourProcessor::FViewSingleFloorProcessor>(
		                                          [this](
		                                          auto NewProcessor
		                                          )
		                                          {
			                                          NewProcessor->Interaction_Area =
				                                          GetBranchDecoratorType();
		                                          }
		                                         );
}

void FFloor_Decorator::Quit()
{
	USceneInteractionWorldSystem::GetInstance()->ClearFocus();
	USceneInteractionWorldSystem::GetInstance()->ClearRouteMarker();

	if (Building_Floor_Mask)
	{
		Building_Floor_Mask->Destroy();
	}
	Building_Floor_Mask = nullptr;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this](
	                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
	                                                                  )
	                                                                  {
		                                                                  // SPtr->UpdateViewConfig(Config);
	                                                                  },
	                                                                  false
	                                                                 );
	Super::Quit();
}

void FFloor_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		{
			auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
				 USmartCitySuiteTags::Interaction_Mode
				);
			if (
				DecoratorSPtr
			)
			{
				if (
					DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
				)
				{
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
					                                                      ))
					{
						return;
					}
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
					                                                      ))
					{
						return;
					}
				}
			}
		}
	}

	bool bUseTemporaComfig = false;

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 30;
			                                                                  Config.PillarTranlucent = 30;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	TSet<TObjectPtr<ASceneElementBase>> SceneElementSet;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();
	};

	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	ON_SCOPE_EXIT
	{
		TSet<FGameplayTag> FloorSet = GetFloorSet();

		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Floor(
		                                                                SceneActorConditional,
		                                                                true,
		                                                                false,
		                                                                FloorSet,
		                                                                MulticastDelegate,
		                                                                SceneElementSet
		                                                               );
	};

	if (
		NewDecoratorSPtr
	)
	{
		if (
			NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                          USmartCitySuiteTags::Interaction_Mode_BatchControl
			                                                         ))
			{
			}
		}
	}

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_ViewElevator
		                                                         ))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EmergencySystem))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();

				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
		                                                         ))
		{
			Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();
				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}

		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EnergyManagement))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorViewEnergyProcessor>(
				                                                    [ this](
				                                                    auto NewProcessor
				                                                    )
				                                                    {
					                                                    NewProcessor->Interaction_Area =
						                                                    GetBranchDecoratorType();
				                                                    }
				                                                   );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_View))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [ this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger_ELV_Radar))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [ this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_SafeManagement))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [ this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_BatchControl))
		{
			auto BatchControlMode_DecoratorSPtr = DynamicCastSharedPtr<FBatchControlMode_Decorator>(NewDecoratorSPtr);
			for (auto Iter : BatchControlMode_DecoratorSPtr->SceneElementSet)
			{
				SceneElementSet.Add(Iter);
			}

			SceneActorConditional.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Mode_BatchControl);

			// MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_TemperatureMap))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [ this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}

		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
				TourProcessor::FViewSingleFloorProcessor>(
				                                          [this](
				                                          auto NewProcessor
				                                          )
				                                          {
					                                          NewProcessor->Interaction_Area =
						                                          GetBranchDecoratorType();
				                                          }
				                                         );

			return;
		}
	}

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
		auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			 USmartCitySuiteTags::Interaction_Mode
			);
		if (
			DecoratorSPtr
		)
		{
			SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());
		}

		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
			TourProcessor::FViewSingleFloorProcessor>(
			                                          [this](
			                                          auto NewProcessor
			                                          )
			                                          {
				                                          NewProcessor->Interaction_Area =
					                                          GetBranchDecoratorType();
			                                          }
			                                         );

		return;
	}

	SceneActorConditional.ConditionalSet = USceneInteractionWorldSystem::GetInstance()->GetAllInteractionTags();

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
		TourProcessor::FViewSingleFloorProcessor>(
		                                          [this](
		                                          auto NewProcessor
		                                          )
		                                          {
			                                          NewProcessor->Interaction_Area =
				                                          GetBranchDecoratorType();
		                                          }
		                                         );
}

bool FFloor_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	Super::Operation(OperatorType);

	return false;
}

void FFloor_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorIter.Value->FloorTag.MatchesTag(GetBranchDecoratorType()))
		{
			if (Building_Floor_Mask)
			{
			}
			else
			{
				Building_Floor_Mask = GetWorldImp()->SpawnActor<ABuilding_Floor_Mask>(
					 UAssetRefMap::GetInstance()->Building_Floor_MaskClass
					);
			}
			if (Building_Floor_Mask)
			{
				Building_Floor_Mask->SetFloor(FloorIter.Value.LoadSynchronous());
			}

			break;
		}
	}
}

void FFloor_Decorator::Process()
{
}

void FFloor_Decorator::AdjustCamera() const
{
	auto Floor = GetBranchDecoratorType();

	if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13))
	{
		Floor = USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall;
	}
}

TSet<FGameplayTag> FFloor_Decorator::GetFloorSet() const
{
	TSet<FGameplayTag> FloorSet;

	if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall);
	}
	else if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
	}
	else if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13J))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
	}
	else if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1) ||
		GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2) ||
		GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3)
	)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1) 
		)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1Common);
	}
	else if (
		GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1J) 
		)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1Common);
	}
	else
	{
		FloorSet.Add(GetBranchDecoratorType());
	}

	return FloorSet;
}

FViewDevice_Decorator::FViewDevice_Decorator(
	) :
	  Super(
	       )
{
}

void FViewDevice_Decorator::Entry()
{
	Super::Entry();

	Process();
}

void FViewDevice_Decorator::ReEntry()
{
	Super::ReEntry();

	Process();
}

void FViewDevice_Decorator::Quit()
{
	if (Building_Floor_Mask)
	{
		Building_Floor_Mask->Destroy();
	}
	Building_Floor_Mask = nullptr;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this](
	                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
	                                                                  )
	                                                                  {
		                                                                  SPtr->ClearTemporaViewConfig();
	                                                                  },
	                                                                  false
	                                                                 );
	Super::Quit();
}

void FViewDevice_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		{
			auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
				 USmartCitySuiteTags::Interaction_Mode
				);
			if (
				DecoratorSPtr
			)
			{
				if (
					DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
				)
				{
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
					                                                      ))
					{
						return;
					}
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
					                                                      ))
					{
						return;
					}
				}
			}
		}
	}

	bool bUseTemporaComfig = false;

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 30;
			                                                                  Config.PillarTranlucent = 30;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	FSceneElementConditional SceneActorConditional;

	TSet<FGameplayTag> FloorSet = GetFloorSet();

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();

		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Device(
		                                                                 SceneActorConditional,
		                                                                 true,
		                                                                 false,
		                                                                 FloorSet,
		                                                                 MulticastDelegate,
		                                                                 SceneElementPtr
		                                                                );

		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
			TourProcessor::FViewSingleDeviceProcessor>(
			                                           [this](
			                                           auto NewProcessor
			                                           )
			                                           {
				                                           NewProcessor->SceneElementPtr = SceneElementPtr;
			                                           }
			                                          );
	};

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_ViewElevator
		                                                         ))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EmergencySystem))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();

				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
		                                                         ))
		{
			Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			IncreaseWaitTaskCount();

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();
				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}

		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EnergyManagement))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_View))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger_ELV_Radar))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}

		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
	}

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
		auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			 USmartCitySuiteTags::Interaction_Mode
			);
		if (
			DecoratorSPtr
		)
		{
			SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());
		}

		return;
	}

	SceneActorConditional.ConditionalSet = USceneInteractionWorldSystem::GetInstance()->GetAllInteractionTags();

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
}

void FViewDevice_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

void FViewDevice_Decorator::Process()
{
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateWeather(WeatherSettings::Clear_Skies);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverageMunualOverride(true);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverage(0);

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	bool bUseTemporaComfig = false;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 10;
			                                                                  Config.PillarTranlucent = 10;
			                                                                  Config.StairsTranlucent = 10;
			                                                                  Config.CurtainWallTranlucent = 10;
			                                                                  Config.FurnitureTranlucent = 10;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();

		TSet<FGameplayTag> FloorSet = GetFloorSet();


		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Device(
		                                                                 SceneActorConditional,
		                                                                 true,
		                                                                 true,
		                                                                 FloorSet,
		                                                                 MulticastDelegate,
		                                                                 SceneElementPtr
		                                                                );

		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
			TourProcessor::FViewSingleDeviceProcessor>(
			                                           [this](
			                                           auto NewProcessor
			                                           )
			                                           {
				                                           NewProcessor->SceneElementPtr = SceneElementPtr;
			                                           }
			                                          );

		AdjustCamera();
	};

	auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
		 USmartCitySuiteTags::Interaction_Mode
		);
	if (
		DecoratorSPtr
	)
	{
		if (
			DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().
			                   MatchesTag(USmartCitySuiteTags::Interaction_Mode_ViewElevator))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EnergyManagement
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);
				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
		}
	}

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
}

void FViewDevice_Decorator::AdjustCamera() const
{
}

TSet<FGameplayTag> FViewDevice_Decorator::GetFloorSet() const
{
	TSet<FGameplayTag> FloorSet;
	
	if (
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13Common)
	)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall);
	}
	else if (SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3)
	)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1J) ||
		SceneElementPtr->BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1Common) 
		)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1Common);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1);
	}
	else
	{
		FloorSet.Add(SceneElementPtr->BelongFloor->FloorTag);
	}

	return FloorSet;
}

FViewSpace_Decorator::FViewSpace_Decorator() :
                                             Super(
                                                  )
{
}

void FViewSpace_Decorator::Entry()
{
	Super::Entry();

	Process();
}

void FViewSpace_Decorator::ReEntry()
{
	Super::ReEntry();

	Process();
}

void FViewSpace_Decorator::Quit()
{
	if (Building_Floor_Mask)
	{
		Building_Floor_Mask->Destroy();
	}
	Building_Floor_Mask = nullptr;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this](
	                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
	                                                                  )
	                                                                  {
		                                                                  SPtr->ClearTemporaViewConfig();
	                                                                  },
	                                                                  false
	                                                                 );
	Super::Quit();
}

void FViewSpace_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	Super::OnOtherDecoratorEntry(NewDecoratorSPtr);

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		{
			auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
				 USmartCitySuiteTags::Interaction_Mode
				);
			if (
				DecoratorSPtr
			)
			{
				if (
					DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
				)
				{
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
					                                                      ))
					{
						return;
					}
					if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
					                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
					                                                      ))
					{
						return;
					}
				}
			}
		}
	}

	bool bUseTemporaComfig = false;

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 30;
			                                                                  Config.PillarTranlucent = 30;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	TSet<TObjectPtr<ASceneElementBase>> SceneElementSet;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();

		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
			TourProcessor::FViewSingleSpaceProcessor>(
			                                          [this](
			                                          auto NewProcessor
			                                          )
			                                          {
				                                          NewProcessor->SceneElementPtr = SceneElementPtr;
			                                          }
			                                         );
	};

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_BatchControl))

		{
			// return;
		}
	}

	ON_SCOPE_EXIT
	{
		TSet<FGameplayTag> FloorSet = GetFloorSet();

		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Space(
		                                                                SceneActorConditional,
		                                                                true,
		                                                                false,
		                                                                FloorSet,
		                                                                MulticastDelegate,
		                                                                SceneElementPtr,
		                                                                SceneElementSet
		                                                               );
	};

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
	)
	{
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_ViewElevator
		                                                         ))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EmergencySystem))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();

				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		                                                          USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
		                                                         ))
		{
			Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			bUseTemporaComfig = true;
			USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
			                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
			                                                                  [this,&Config](
			                                                                  const TSharedPtr<FInteraction_Decorator>&
			                                                                  SPtr
			                                                                  )
			                                                                  {
				                                                                  Config = SPtr->GetViewConfig();
				                                                                  Config.WallTranlucent = 100;
				                                                                  Config.PillarTranlucent = 100;
			                                                                  },
			                                                                  false
			                                                                 );

			return;
		}

		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_EnergyManagement))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_View))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger_ELV_Radar))
		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
		if (NewDecoratorSPtr->GetBranchDecoratorType().
		                      MatchesTag(USmartCitySuiteTags::Interaction_Mode_BatchControl))
		{
			auto BatchControlMode_DecoratorSPtr = DynamicCastSharedPtr<FBatchControlMode_Decorator>(NewDecoratorSPtr);
			for (auto Iter : BatchControlMode_DecoratorSPtr->SceneElementSet)
			{
				SceneElementSet.Add(Iter);
			}

			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}

		{
			SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
			SceneActorConditional.ConditionalSet.AddTag(NewDecoratorSPtr->GetBranchDecoratorType());

			MulticastDelegate.AddRaw(NewDecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);

			return;
		}
	}

	if (
		NewDecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Interaction)
	)
	{
		SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
		auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			 USmartCitySuiteTags::Interaction_Mode
			);
		if (
			DecoratorSPtr
		)
		{
			SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());
		}

		return;
	}

	SceneActorConditional.ConditionalSet = USceneInteractionWorldSystem::GetInstance()->GetAllInteractionTags();

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
}

void FViewSpace_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

bool FViewSpace_Decorator::Operation(
	EOperatorType OperatorType
	)
{
	return false;
}

void FViewSpace_Decorator::Process()
{
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateWeather(WeatherSettings::Clear_Skies);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverageMunualOverride(true);
	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverage(0);

	FDateTime Time(1, 1, 1, 12);

	FViewConfig Config;

	bool bUseTemporaComfig = false;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this,&Config](
	                                                                  const TSharedPtr<FInteraction_Decorator>&
	                                                                  SPtr
	                                                                  )
	                                                                  {
		                                                                  Config = SPtr->GetViewConfig();

		                                                                  if (SPtr->HasViewConfigChanged())
		                                                                  {
		                                                                  }
		                                                                  else
		                                                                  {
			                                                                  Config.WallTranlucent = 10;
			                                                                  Config.PillarTranlucent = 10;
			                                                                  Config.StairsTranlucent = 10;
			                                                                  Config.CurtainWallTranlucent = 10;
			                                                                  Config.FurnitureTranlucent = 10;
			                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  SPtr->UpdateViewConfig(Config, false);
		                                                                  }
	                                                                  },
	                                                                  false
	                                                                 );

	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	TSet<TObjectPtr<ASceneElementBase>> SceneElementSet;

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this,&Config, &bUseTemporaComfig](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  if (bUseTemporaComfig)
			                                                                  {
				                                                                  SPtr->UpdateViewConfig(Config, true);
			                                                                  }
			                                                                  else
			                                                                  {
				                                                                  SPtr->ClearTemporaViewConfig();
			                                                                  }
		                                                                  },
		                                                                  false
		                                                                 );

		UWeatherSystem::GetInstance()->ResetTime();

		SceneActorConditional.ConditionalSet.AddTag(Floor);

		UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<
			TourProcessor::FViewSingleSpaceProcessor>(
			                                          [this](
			                                          auto NewProcessor
			                                          )
			                                          {
				                                          NewProcessor->SceneElementPtr = SceneElementPtr;
			                                          }
			                                         );

		AdjustCamera();
	};

	auto DecoratorSPtr = USceneInteractionWorldSystem::GetInstance()->GetDecorator(
		 USmartCitySuiteTags::Interaction_Mode
		);

	if (
		DecoratorSPtr
	)
	{
		if (
			DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_BatchControl))
			{
				USceneInteractionWorldSystem::GetInstance()->SwitchInteractionMode(
					 USmartCitySuiteTags::Interaction_Mode_Empty
					);
			}
		}
	}

	ON_SCOPE_EXIT
	{
		TSet<FGameplayTag> FloorSet = GetFloorSet();

		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_Space(
		                                                                SceneActorConditional,
		                                                                true,
		                                                                true,
		                                                                FloorSet,
		                                                                MulticastDelegate,
		                                                                SceneElementPtr,
		                                                                SceneElementSet
		                                                               );
	};

	if (
		DecoratorSPtr
	)
	{
		if (
			DecoratorSPtr->GetMainDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode)
		)
		{
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Mode_Empty))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().
			                   MatchesTag(USmartCitySuiteTags::Interaction_Mode_ViewElevator))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EmergencySystem
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_DeviceManagger_PWR_Lighting
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				Time = FDateTime(1, 1, UAssetRefMap::GetInstance()->ViewLightingTime);

				bUseTemporaComfig = true;
				USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
					 USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
					 [this,&Config](
					 const TSharedPtr<FInteraction_Decorator>& SPtr
					 )
					 {
						 Config = SPtr->GetViewConfig();

						 Config.WallTranlucent = 100;
						 Config.PillarTranlucent = 100;
					 },
					 false
					);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_EnergyManagement
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
			if (DecoratorSPtr->GetBranchDecoratorType().MatchesTag(
			                                                       USmartCitySuiteTags::Interaction_Mode_BatchControl
			                                                      ))
			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());

				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}

			{
				SceneActorConditional.ConditionalSet.AddTag(GetBranchDecoratorType());
				SceneActorConditional.ConditionalSet.AddTag(DecoratorSPtr->GetBranchDecoratorType());

				MulticastDelegate.AddRaw(DecoratorSPtr.Get(), &FDecoratorBase::OnUpdateFilterComplete);
				MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

				return;
			}
		}
	}

	SceneActorConditional.ConditionalSet.AddTag(Floor);
	SceneActorConditional.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Area_Space);

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);
}

void FViewSpace_Decorator::AdjustCamera() const
{
}

TSet<FGameplayTag> FViewSpace_Decorator::GetFloorSet() const
{
	TSet<FGameplayTag> FloorSet;

	if (Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13MultFunctionHall);
	}
	else if (Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Bathroom);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
	}
	else if (Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F13J))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F13Common);
	}
	else if (Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1) ||
		Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2) ||
		Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3)
	)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F12JF))
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_2);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F12JF_3);
	}
	else if (
		Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1) 
		)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1Common);
	}
	else if (
		Floor.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_F1J) 
		)
	{
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1J);
		FloorSet.Add(USmartCitySuiteTags::Interaction_Area_Floor_F1Common);
	}
	else
	{
		FloorSet.Add(Floor);
		FloorSet.Add(GetBranchDecoratorType());
	}

	return FloorSet;
}

void FViewSpecialArea_Decorator::Entry()
{
	Super::Entry();

	UInputProcessorSubSystem_Imp::GetInstance()->SwitchToProcessor<TourProcessor::FViewSpecialAreaProcessor>(
		 [](
		 auto NewProcessor
		 )
		 {
		 }
		);

	auto DecoratorSPtr =
		DynamicCastSharedPtr<FInteraction_Decorator>(
		                                             USceneInteractionWorldSystem::GetInstance()->
		                                             GetDecorator(
		                                                          USmartCitySuiteTags::Interaction_Interaction
		                                                         )
		                                            );
	if (DecoratorSPtr)
	{
		USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
		                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
		                                                                  [this](
		                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
		                                                                  )
		                                                                  {
			                                                                  auto TempConfig = SPtr->GetViewConfig();
			                                                                  TempConfig.WallTranlucent = 10;
			                                                                  TempConfig.PillarTranlucent = 10;
			                                                                  TempConfig.StairsTranlucent = 10;
			                                                                  TempConfig.CurtainWallTranlucent = 10;
			                                                                  TempConfig.FurnitureTranlucent = 10;

			                                                                  SPtr->UpdateViewConfig(TempConfig, true);
		                                                                  },
		                                                                  false
		                                                                 );
	}

	Process();
}

void FViewSpecialArea_Decorator::ReEntry()
{
	Super::ReEntry();

	Process();
}

void FViewSpecialArea_Decorator::Quit()
{
	if (PreviousPtr)
	{
		PreviousPtr->QuitAllState();
	}
	PreviousPtr = nullptr;

	USceneInteractionWorldSystem::GetInstance()->SetInteractionOption(
	                                                                  USmartCitySuiteTags::Interaction_Interaction_WallTranlucent,
	                                                                  [this](
	                                                                  const TSharedPtr<FInteraction_Decorator>& SPtr
	                                                                  )
	                                                                  {
		                                                                  SPtr->ClearTemporaViewConfig();
	                                                                  },
	                                                                  false
	                                                                 );
	Super::Quit();
}

void FViewSpecialArea_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	Super::OnUpdateFilterComplete(bIsOK, TaskPtr);
}

void FViewSpecialArea_Decorator::Process()
{
	FSceneElementConditional SceneActorConditional;

	for (auto Iter : FloorSet)
	{
		SceneActorConditional.ConditionalSet.AddTag(Iter);
	}
	SceneActorConditional.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Area_SpecialArea);

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddRaw(this, &ThisClass::OnUpdateFilterComplete);

	USceneInteractionWorldSystem::GetInstance()->UpdateFilter_SpeacialArea(
	                                                                       SceneActorConditional,
	                                                                       true,
	                                                                       true,
	                                                                       MulticastDelegate,
	                                                                       FloorSet,
	                                                                       PriorityHideFloorSet
	                                                                      );

	AdjustCamera();
}

void FViewSpecialArea_Decorator::AdjustCamera()
{
}
