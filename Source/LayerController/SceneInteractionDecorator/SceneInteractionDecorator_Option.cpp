#include "SceneInteractionDecorator_Option.h"

#include "AssetRefMap.h"
#include "GameOptions.h"
#include "MessageBody.h"
#include "SceneInteractionWorldSystem.h"
#include "Dynamic_WeatherBase.h"
#include "TemplateHelper.h"
#include "SmartCitySuiteTags.h"
#include "WeatherSystem.h"
#include "SceneInteractionDecorator_Area.h"

void FInteraction_Decorator::SwitchIteractionType(
	EInteractionType NewInteractionType
	)
{
	ControllConfig.InteractionType = NewInteractionType;
}

EInteractionType FInteraction_Decorator::GetInteractionType() const
{
	return ControllConfig.InteractionType;
}

FGameplayTag FInteraction_Decorator::GetCurrentWeather() const
{
	return ControllConfig.CurrentWeather;
}

void FInteraction_Decorator::SetCurrentWeather(
	const FGameplayTag& WeatherTag
	)
{
	ControllConfig.CurrentWeather = WeatherTag;

	auto AreaDecoratorSPtr =
		DynamicCastSharedPtr<FArea_Decorator>(
		                                      USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			                                       USmartCitySuiteTags::Interaction_Area
			                                      )
		                                     );

	if (AreaDecoratorSPtr && AreaDecoratorSPtr->GetBranchDecoratorType().MatchesTag(
		     USmartCitySuiteTags::Interaction_Area_Floor
		    ))
	{
		return;
	}

	UWeatherSystem::GetInstance()->GetDynamicWeather()->UpdateWeather(ControllConfig.CurrentWeather);
}

int32 FInteraction_Decorator::GetCurrentHour() const
{
	return ControllConfig.CurrentHour;
}

void FInteraction_Decorator::SetCurrentHour(
	int32 Hour
	)
{
	ControllConfig.CurrentHour = Hour;

	FDateTime Time(1, 1, 1, Hour);

	UWeatherSystem::GetInstance()->AdjustTime(Time);
}

void FInteraction_Decorator::UpdateViewConfig(
	const FViewConfig& InConfig,
	bool bIsTempora
	)
{
}

void FInteraction_Decorator::ClearTemporaViewConfig()
{
	TemporaViewConfigAry.Empty();
}

FViewConfig FInteraction_Decorator::GetViewConfig() const
{
	if (TemporaViewConfigAry.IsEmpty())
	{
	}
	else
	{
		return TemporaViewConfigAry[0];
	}

	if (ViewConfig.IsEmpty())
	{
	}
	else
	{
		return ViewConfig[0];
	}

	return FViewConfig();
}

bool FInteraction_Decorator::HasViewConfigChanged() const
{
	return ViewConfig.Num() > 0;
}

void FInteraction_Decorator::UpdateControlConfig(
	const FControlConfig& InConfig
	)
{
	ControllConfig = InConfig;
}

FControlConfig FInteraction_Decorator::GetConfigControlConfig() const
{
	return ControllConfig;
}
