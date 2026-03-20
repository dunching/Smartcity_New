#include "HumanRegularProcessor.h"

#include "DrawDebugHelpers.h"
#include "Async/Async.h"
#include "Engine/LocalPlayer.h"

#include "InputProcessorSubSystemBase.h"

#include "IPSSI.h"

static TAutoConsoleVariable<int32> HumanRegularProcessor(
                                                         TEXT("Skill.DrawDebug.HumanRegularProcessor"),
                                                         0,
                                                         TEXT("")
                                                         TEXT(" default: 0")
                                                        );

namespace HumanProcessor
{
	FHumanRegularProcessor::~FHumanRegularProcessor()
	{
	}

	void FHumanRegularProcessor::EnterAction()
	{
		Super::EnterAction();

		Async(
		      EAsyncExecution::ThreadPool,
		      std::bind(&ThisClass::UpdateLookAtObject, this)
		     );

		SwitchCurrentWeapon();

		{
			SwitchShowCursor(false);

			AddOrRemoveUseMenuItemEvent(true);
		}
	}

	void FHumanRegularProcessor::TickImp(
		float Delta
		)
	{
		Super::TickImp(Delta);


	}

	void FHumanRegularProcessor::SwitchWeapon()
	{
	}

	void FHumanRegularProcessor::SwitchCurrentWeapon()
	{
	}

	void FHumanRegularProcessor::QuitAction()
	{
		AddOrRemoveUseMenuItemEvent(false);


		Super::QuitAction();
	}

	bool FHumanRegularProcessor::InputKey(
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
			}
			break;
		}

		return Super::InputKey(EventArgs);
	}

	bool FHumanRegularProcessor::InputAxis(
		const FInputKeyEventArgs& EventArgs
		)
	{
		return Super::InputAxis(EventArgs);
	}

	void FHumanRegularProcessor::AddOrRemoveUseMenuItemEvent(
		bool bIsAdd
		)
	{
	}

	void FHumanRegularProcessor::UpdateLookAtObject()
	{
		IncreaseAsyncTaskNum();
		ON_SCOPE_EXIT
		{
			ReduceAsyncTaskNum();
		};
	}
}
