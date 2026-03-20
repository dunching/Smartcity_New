#include "ViewSpecialAreaProcessor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameOptions.h"
#include "PlayerGameplayTasks.h"
#include "SceneInteractionWorldSystem.h"

void TourProcessor::FViewSpecialAreaProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	SwitchShowCursor(true);

	SwitchRender(false);
}

bool TourProcessor::FViewSpecialAreaProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSpecialAreaProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}
