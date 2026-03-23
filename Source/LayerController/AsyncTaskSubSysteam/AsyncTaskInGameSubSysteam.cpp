#include "AsyncTaskInGameSubSysteam.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"

UAsyncTaskInGameSubSysteam* UAsyncTaskInGameSubSysteam::GetInstance()
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

	return Cast<UAsyncTaskInGameSubSysteam>(
	                                  USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
		                                   GetWorldImp(),
		                                   UAsyncTaskInGameSubSysteam::StaticClass()
		                                  )
	                                 );
}
