// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TT_SwitchSceneElement.generated.h"

class IQueuedWork_ProcessTasks;

USTRUCT()
struct LAYERCONTROLLER_API FThreadTaskBase
{
	GENERATED_BODY()

public:
	friend IQueuedWork_ProcessTasks;
	
	FThreadTaskBase();
	
	virtual ~FThreadTaskBase();

	virtual void Activate();

	virtual void TickTask(
		float DeltaTime
		);

	virtual void ExternalConfirm(
		bool bEndTask
		);

	virtual void ExternalCancel();

	virtual void OnDestroy(
		bool bInOwnerFinished
		);

	TFunction<void()> OnComplete;

private:
	EGameplayTaskState TaskState = EGameplayTaskState::AwaitingActivation;
};

USTRUCT()
struct LAYERCONTROLLER_API FThreadTaskFunction : public FThreadTaskBase
{
	GENERATED_BODY()

public:
	virtual void TickTask(
		float DeltaTime
		) override;
	
	TFunction<bool()> Func;
};
