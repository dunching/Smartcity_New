// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GTComponent.h"
#include "HumanProcessor.h"
#include "PlayerControllerBase.h"

#include "AsyncTaskInGameSubSysteam.generated.h"

class FEvent;

class IQueuedWork_ProcessTasks;
class FTaskGroup;

/*
 *
 */
UCLASS(BlueprintType, Blueprintable)
class LAYERCONTROLLER_API UAsyncTaskInGameSubSysteam : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncTaskInGameSubSysteam* GetInstance();

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

private:
};

template <typename GameplayTaskType>
GameplayTaskType* UAsyncTaskInGameSubSysteam::StartGameplayTask(
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
GameplayTaskType* UAsyncTaskInGameSubSysteam::AddGameplayDelayTask(
	const std::function<void(
		GameplayTaskType*


		
		)>& Func
	)
{
	return nullptr;
}