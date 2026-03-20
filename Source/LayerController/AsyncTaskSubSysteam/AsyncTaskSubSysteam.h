// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GTComponent.h"
#include "HumanProcessor.h"
#include "PlayerControllerBase.h"

#include "AsyncTaskSubSysteam.generated.h"

/**
 */
UCLASS(BlueprintType, Blueprintable)
class LAYERCONTROLLER_API UAsyncTaskSubSysteam : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncTaskSubSysteam* GetInstance();

	template <typename GameplayTaskType>
	GameplayTaskType* StartGameplayTask(
		bool bBreakRuntimeTask,
		bool bBreakCameraRuntimeTask,
		const std::function<void(
			GameplayTaskType*


			
			)>& Func
		);

	template <typename GameplayTaskType>
	GameplayTaskType* AddGameplayDelayTask(
		const std::function<void(
			GameplayTaskType*


			
			)>& Func
		);


	template <typename ThreadTaskType>
	ThreadTaskType* StartThreadTask(
		bool bBreakRuntimeTask,
		bool bBreakCameraRuntimeTask,
		const std::function<void(
			ThreadTaskType*
			)>& Func
		);

private:
};

template <typename GameplayTaskType>
GameplayTaskType* UAsyncTaskSubSysteam::StartGameplayTask(
	bool bBreakRuntimeTask,
	bool bBreakCameraRuntimeTask,
	const std::function<void(
		GameplayTaskType*


		
		)>& Func
	)
{
	auto PCPtr = Cast<APlayerControllerBase>(GEngine->GetFirstLocalPlayerController(GetWorldImp()));
	return PCPtr->GameplayTasksComponentPtr->StartGameplayTask<GameplayTaskType>(
		 bBreakRuntimeTask,
		 bBreakCameraRuntimeTask,
		 Func
		);
}

template <typename GameplayTaskType>
GameplayTaskType* UAsyncTaskSubSysteam::AddGameplayDelayTask(
	const std::function<void(
		GameplayTaskType*


		
		)>& Func
	)
{
	return nullptr;
}

template <typename ThreadTaskType>
ThreadTaskType* UAsyncTaskSubSysteam::StartThreadTask(
	bool bBreakRuntimeTask,
	bool bBreakCameraRuntimeTask,
	const std::function<void(ThreadTaskType*)>& Func
	)
{
	
	GThreadPool->AddQueuedWork();
	return nullptr;
}
