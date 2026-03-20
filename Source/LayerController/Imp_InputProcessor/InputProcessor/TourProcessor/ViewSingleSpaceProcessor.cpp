#include "ViewSingleSpaceProcessor.h"

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
