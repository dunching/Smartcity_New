#include "TourProcessor.h"

#include "GameFramework/SpringArmComponent.h"

#include "GameOptions.h"
#include "SceneInteractionDecorator_Option.h"
#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

bool TourProcessor::FTourProcessor::UpdateCameraArmLen(
	const FControlParam& ControlParam,
	float OffsetValue
	)
{
	return true;
}

bool TourProcessor::FTourProcessor::UpdateCameraClampPitch(
	const FControlParam& ControlParam
	)
{
	auto DecoratorSPtr =
		DynamicCastSharedPtr<FInteraction_Decorator>(
		                                             USceneInteractionWorldSystem::GetInstance()->
		                                             GetDecorator(
		                                                          USmartCitySuiteTags::Interaction_Interaction
		                                                         )
		                                            );

	if (!DecoratorSPtr)
	{
		return true;
	}

	auto ConfigControlConfig = DecoratorSPtr->GetConfigControlConfig();
	ConfigControlConfig.ProcessPitchMin = ControlParam.CameraPitchMinLimit;
	ConfigControlConfig.ProcessPitchMax = ControlParam.CameraPitchMaxLimit;
	DecoratorSPtr->UpdateControlConfig(ConfigControlConfig);

	return true;
}
