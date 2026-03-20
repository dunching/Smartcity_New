#include "ViewSingleFloorViewEnergyProcessor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameOptions.h"
#include "SceneInteractionWorldSystem.h"

void TourProcessor::FViewSingleFloorViewEnergyProcessor::EnterAction()
{
	FInputProcessor::EnterAction();

	SwitchShowCursor(true);
	
	SwitchRender(false);
}

bool TourProcessor::FViewSingleFloorViewEnergyProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	switch (EventArgs.Event)
	{
	case IE_Pressed:
		{
		}
		break;
	case IE_Released:
		{
			auto GameOptionsPtr = UGameOptions::GetInstance();

			if (EventArgs.Key == GameOptionsPtr->ClickItem)
			{
				if (bHasRoted || bHasMoved)
				{
				}
				else
				{
					// 如果未旋转或移动、则继续往下
					USceneInteractionWorldSystem::GetInstance()->Operation(EOperatorType::kLeftMouseButton);
				}
			}

			if (EventArgs.Key == GameOptionsPtr->RotBtn)
			{
				bHasRoted = false;

				bStartRot = false;
				return true;
			}

			if (EventArgs.Key == GameOptionsPtr->MoveBtn)
			{
				bHasMoved = false;

				bStartMove = false;
				return true;
			}
		}
		break;
	default: ;
	}

	return Super::InputKey(EventArgs);
}

bool TourProcessor::FViewSingleFloorViewEnergyProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	switch (EventArgs.Event)
	{
	case IE_Axis:
		{
			auto GameOptionsPtr = UGameOptions::GetInstance();
		}
		break;
	default: ;
	}

	return Super::InputKey(EventArgs);
}
