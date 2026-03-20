#include "ViewSingleSpaceProcessor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameOptions.h"
#include "PlayerGameplayTasks.h"
#include "SceneInteractionWorldSystem.h"

void TourProcessor::FViewSingleSpaceProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	SwitchShowCursor(true);

	SwitchRender(false);
}

bool TourProcessor::FViewSingleSpaceProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSingleSpaceProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}
