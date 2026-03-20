#include "ViewSplitFloorProcessor.h"

#include "GameOptions.h"
#include "SceneInteractionDecorator_Area.h"
#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"

void TourProcessor::FViewSplitFloorProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	USceneInteractionWorldSystem::GetInstance()->SwitchDecoratorImp<FSplitFloor_Decorator>(
		 USmartCitySuiteTags::Interaction_Area.GetTag(),
		 USmartCitySuiteTags::Interaction_Area_Floor.GetTag()
		);

}

bool TourProcessor::FViewSplitFloorProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSplitFloorProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}
