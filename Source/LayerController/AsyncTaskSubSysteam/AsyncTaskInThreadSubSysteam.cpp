#include "AsyncTaskInThreadSubSysteam.h"

#include "TT_SwitchSceneElement.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

UAsyncTaskInThreadSubSysteam* UAsyncTaskInThreadSubSysteam::GetInstance()
{
	// FGraphEventRef TaskA = FFunctionGraphTask::CreateAndDispatchWhenReady(
	//                                                                       []()
	//                                                                       {
	//                                                                       },
	//                                                                       TStatId()
	//                                                                      );
	//
	// FGraphEventRef TaskB = FFunctionGraphTask::CreateAndDispatchWhenReady(
	//                                                                       []()
	//                                                                       {
	//                                                                       },
	//                                                                       TStatId(),
	//                                                                       TaskA,
	//                                                                       ENamedThreads::AnyThread
	//                                                                      );
	//
	// FGraphEventArray Tasks;
	// Tasks.Add(TaskA);
	// Tasks.Add(TaskB);
	//
	// FFunctionGraphTask::CreateAndDispatchWhenReady(
	//                                                []()
	//                                                {
	// 	                                               UE_LOG(LogTemp, Warning, TEXT("All Done"));
	//                                                },
	//                                                TStatId(),
	//                                                &Tasks,
	//                                                ENamedThreads::GameThread
	//                                               );

	return Cast<UAsyncTaskInThreadSubSysteam>(
	                                          USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
		                                           GetWorldImp(),
		                                           UAsyncTaskInThreadSubSysteam::StaticClass()
		                                          )
	                                         );
}

void IQueuedWork_ProcessTasks::DoThreadedWork()
{
	auto LastTickTime = FPlatformTime::Seconds();

	for (; TaskGroupPtr;)
	{
		ON_SCOPE_EXIT
		{
			LastTickTime = FPlatformTime::Seconds();
		};

		for (auto Iter = TaskGroupPtr->TaskGroupsAry.CreateIterator(); Iter; ++Iter)
		{
			switch (Iter->TaskState)
			{
			case EGameplayTaskState::Uninitialized:
				break;
			case EGameplayTaskState::AwaitingActivation:
				{
					Iter->Activate();
					Iter->TaskState = EGameplayTaskState::Active;
				}
				break;
			case EGameplayTaskState::Paused:
				break;
			case EGameplayTaskState::Active:
				{
					auto CurrentTime = FPlatformTime::Seconds();
					Iter->TickTask(CurrentTime - LastTickTime);
				}
				break;
			case EGameplayTaskState::Finished:
				{
					Iter.RemoveCurrent();
				}
				break;
			}
		}
		
		if (TaskGroupPtr->TaskGroupsAry.IsEmpty())
		{
			TaskGroupPtr->QueuedWork_ProcessTaskPtr = nullptr;
			delete this;
			return;
		}
	}
}

void IQueuedWork_ProcessTasks::Abandon()
{
	delete this;
}

void UAsyncTaskInThreadSubSysteam::StartThreadTask(
	bool bBreakRuntimeTask,
	bool bBreakCameraRuntimeTask,
	const FThreadTaskBase& ThreadTask
	)
{
	if (TaskGroupAry.Num() < MaxThreadNum)
	{
		auto SPtr = MakeShared<FTaskGroup>();
		
		SPtr->TaskGroupsAry.Add(ThreadTask);
		SPtr->QueuedWork_ProcessTaskPtr = new IQueuedWork_ProcessTasks;
		
		GThreadPool->AddQueuedWork(SPtr->QueuedWork_ProcessTaskPtr);

		TaskGroupAry.Add(SPtr);
	}
	else
	{
		int32 MinInde = 0;
		for (int32 Index = 1; Index < TaskGroupAry.Num(); Index++)
		{
			if (TaskGroupAry[Index]->TaskGroupsAry.Num() < TaskGroupAry[MinInde]->TaskGroupsAry.Num())
			{
				MinInde = Index;
			}
		}
		
		if (!TaskGroupAry[MinInde]->QueuedWork_ProcessTaskPtr)
		{
			TaskGroupAry[MinInde]->QueuedWork_ProcessTaskPtr = new IQueuedWork_ProcessTasks;
		
			GThreadPool->AddQueuedWork(TaskGroupAry[MinInde]->QueuedWork_ProcessTaskPtr);
		}
	}
}
