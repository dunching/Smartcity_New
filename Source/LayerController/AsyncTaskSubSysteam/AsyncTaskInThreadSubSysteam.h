// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GTComponent.h"
#include "HumanProcessor.h"
#include "PlayerControllerBase.h"

#include "AsyncTaskInThreadSubSysteam.generated.h"

class FEvent;

class IQueuedWork_ProcessTasks;
class FTaskGroup;

struct FThreadTaskBase;

/*
 *
 */
UCLASS(BlueprintType, Blueprintable)
class LAYERCONTROLLER_API UAsyncTaskInThreadSubSysteam : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncTaskInThreadSubSysteam* GetInstance();
	
	void StartThreadTask(
		bool bBreakRuntimeTask,
		bool bBreakCameraRuntimeTask,
		const FThreadTaskBase& ThreadTask
		);

private:
	
	TArray<TSharedPtr<FTaskGroup>>TaskGroupAry;
	
	const uint8 MaxThreadNum = 2;
};

class FTaskGroup
{
public:
	
	TArray<FThreadTaskBase> TaskGroupsAry;
	
	IQueuedWork_ProcessTasks*QueuedWork_ProcessTaskPtr = nullptr;
	
	FEvent* OnTaskCompletedEvent;

	FCriticalSection ThreadPoolMutex;
	
	FCriticalSection FenceMutex;
	
};

class IQueuedWork_ProcessTasks : public IQueuedWork
{
public:

	TSharedPtr<FTaskGroup> TaskGroupPtr = nullptr;
	
	virtual void DoThreadedWork() override;

	virtual void Abandon() override;
};
