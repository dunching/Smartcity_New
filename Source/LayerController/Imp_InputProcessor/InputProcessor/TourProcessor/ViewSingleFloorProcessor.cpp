#include "ViewSingleFloorProcessor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameOptions.h"
#include "SceneInteractionWorldSystem.h"
 
void TourProcessor::FViewSingleFloorProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	SwitchShowCursor(true);
	
	SwitchRender(false);
}

bool TourProcessor::FViewSingleFloorProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSingleFloorProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}
