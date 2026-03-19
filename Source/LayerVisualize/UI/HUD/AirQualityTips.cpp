#include "AirQualityTips.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UAirQualityTips::NativeConstruct()
{
	Super::NativeConstruct();
}

FVector UAirQualityTips::GetHoverPosition()
{
	return TargetPt;
}

void UAirQualityTips::UpdateValue(float Value)
{
	if (Value > 250)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else if (Value > 150)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		checkNoEntry()
	}
}
