#include "ViewTowerProcessor.h"

#include "Dynamic_WeatherBase.h"

#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"
#include "WeatherSystem.h"

void TourProcessor::FViewTowerProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	// USceneInteractionWorldSystem::GetInstance()->SwitchDecoratorImp<FViewTower_Decorator>(
	// 	 USmartCitySuiteTags::Interaction_Area.GetTag(),
	// 	 USmartCitySuiteTags::Interaction_Area_ExternalWall.GetTag()
	// 	);

	AdjustCamera();

	{
		// 确认当前的模式
		auto DecoratorSPtr =
			DynamicCastSharedPtr<FInteraction_Decorator>(
			                                             USceneInteractionWorldSystem::GetInstance()->
			                                             GetDecorator(
			                                                          USmartCitySuiteTags::Interaction_Interaction
			                                                         )
			                                            );
		if (DecoratorSPtr)
		{
			UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateWeather(DecoratorSPtr->GetCurrentWeather());
			UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateCloudCoverageMunualOverride(false);

			FDateTime Time(1, 1, 1, DecoratorSPtr->GetCurrentHour());
			UWeatherSystem::GetInstance()->ResetTime();
		}
	}

	SwitchShowCursor(true);
	
	SwitchRender(true);

}

bool TourProcessor::FViewTowerProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewTowerProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

void TourProcessor::FViewTowerProcessor::AdjustCameraSeat(
	const FRotator& CameraSeat
	)
{
}

void TourProcessor::FViewTowerProcessor::AdjustCamera()
{
}
