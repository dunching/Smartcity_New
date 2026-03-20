
#include "HumanProcessor.h"

#include "DrawDebugHelpers.h"
#include "Async/Async.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

#include "GameOptions.h"

#ifdef WITH_EDITOR
static TAutoConsoleVariable<int32> DrawDebugHumanProcessor(
	TEXT("Skill.DrawDebug.HumanProcessor"),
	0,
	TEXT("")
	TEXT(" default: 0"));
#endif

namespace HumanProcessor
{
	void FHumanProcessor::EnterAction()
	{
		Super::EnterAction();
	}

	bool FHumanProcessor::InputKey(
		const FInputKeyEventArgs& EventArgs
	)
	{
		switch (EventArgs.Event)
		{
		case IE_Pressed:
			{
				auto GameOptionsPtr = UGameOptions::GetInstance();
				
			}
			break;
		case IE_Released:
			{
				auto GameOptionsPtr = UGameOptions::GetInstance();
				
			}
			break;
		}

		return FInputProcessor::InputKey(EventArgs);
	}

	void FHumanProcessor::SwitchWalkingOrRunning()
	{
	}

	void FHumanProcessor::Dash()
	{
	}

	void FHumanProcessor::Jump()
	{
	}
}
