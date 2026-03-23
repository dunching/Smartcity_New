#include "TT_SwitchSceneElement.h"

FThreadTaskBase::FThreadTaskBase()
{
}

FThreadTaskBase::~FThreadTaskBase()
{
}

void FThreadTaskBase::Activate()
{
}

void FThreadTaskBase::TickTask(
	float DeltaTime
	)
{
}

void FThreadTaskBase::ExternalConfirm(
	bool bEndTask
	)
{
	OnDestroy(bEndTask);
}

void FThreadTaskBase::ExternalCancel()
{
	OnDestroy(true);
}

void FThreadTaskBase::OnDestroy(
	bool bInOwnerFinished
	)
{
	TaskState = EGameplayTaskState::Finished;

	if (OnComplete)
	{
		OnComplete();
	}
}

void FThreadTaskFunction::TickTask(
	float DeltaTime
	)
{
	Super::TickTask(DeltaTime);
	
	if (Func)
	{
		if (Func())
		{
			
		}
		else
		{
			ExternalConfirm(true);
		}
	}
}
