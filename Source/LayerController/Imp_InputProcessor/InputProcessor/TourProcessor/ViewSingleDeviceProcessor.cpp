#include "ViewSingleDeviceProcessor.h"

void TourProcessor::FViewSingleDeviceProcessor::EnterAction()
{
	FInputProcessor::EnterAction();
}

bool TourProcessor::FViewSingleDeviceProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSingleDeviceProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return Super::InputKey(EventArgs);
}
