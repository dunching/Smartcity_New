#include "ViewSpecialAreaProcessor.h"

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
