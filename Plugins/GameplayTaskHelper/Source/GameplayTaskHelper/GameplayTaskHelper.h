// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>
#include <map>
#include <set>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "GameplayTasksComponent.h"

#include "GameplayTaskHelper.generated.h"

class UGT_CameraTransform;
class AViewerPawnBase;
class USceneInteractionWorldSystem;
class ADatasmithSceneActor;
class AReplaceActorBase;
class UGT_RuntimeTask;
class UGameplayTaskBase;
class ASceneElementBase;
class ASceneElementManaggerBase;
class ASceneElement_DeviceBase;
class ASceneElement_Space;
class ASceneElementCategory;
class UDatasmithAssetUserData;

struct FSceneElementMap;

/*
 * PlayerController处理异步的组件
 */
UCLASS(
	BlueprintType,
	Blueprintable
)
class GAMEPLAYTASKHELPER_API UPlayerControllerGameplayTasksComponent : public UGameplayTasksComponent
{
	GENERATED_BODY()

public:
	static FName ComponentName;

	virtual void OnGameplayTaskDeactivated(
		UGameplayTask& Task
		) override;

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

protected:
	
	UPROPERTY(transient)
	TArray<TObjectPtr<UGameplayTask>> DelayTasks;

};

UCLASS()
class GAMEPLAYTASKHELPER_API UGameplayTaskBase : public UGameplayTask
{
	GENERATED_BODY()

public:
	class UPlayerControllerGameplayTasksComponent;

	using FOnTaskComplete = TMulticastDelegate<void(
		bool
		)>;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGameplayTaskBase> >NextTaskAry;

protected:
	FOnTaskComplete OnTaskComplete;
};

/**
 * 
 */
UCLASS()
class GAMEPLAYTASKHELPER_API UGT_BatchStepBase : public UGameplayTaskBase
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	UGT_BatchStepBase(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void TickTask(
		float DeltaTime
		) override;

protected:
	virtual bool ProcessTask(
		float DeltaTime
		);

	double ScopeTime = .1f;

	enum class EUseScopeType
	{
		kTime,
		kCount,
		kNone,
	};

	EUseScopeType UseScopeType = EUseScopeType::kCount;

	int32 PerTickProcessNum = 100;

private:
};

UCLASS()
class GAMEPLAYTASKHELPER_API UGT_InitializeSceneBase : public UGT_BatchStepBase
{
	GENERATED_BODY()
};

UCLASS()
class GAMEPLAYTASKHELPER_API UGT_RuntimeTask : public UGT_BatchStepBase
{
	GENERATED_BODY()

public:
};

template <typename GameplayTaskType>
GameplayTaskType* UPlayerControllerGameplayTasksComponent::StartGameplayTask(
	bool bBreakRuntimeTask,
	bool bBreakCameraRuntimeTask,
	const std::function<void(
		GameplayTaskType*
		)>& Func
	)
{
	if (bBreakRuntimeTask)
	{
		if constexpr (std::same_as<GameplayTaskType, UGT_InitializeSceneBase>)
		{
		}
		else
		{
			const auto TempKnownTasks = KnownTasks;
			for (auto Iter : TempKnownTasks)
			{
				if (Iter && Iter->IsA(UGT_RuntimeTask::StaticClass()))
				{
					Iter->EndTask();
				}
			}
			
			const auto TempDelayTasks = DelayTasks;
			for (auto Iter : TempDelayTasks)
			{
				Iter->EndTask();
			}
			DelayTasks.Empty();
		}
	}

	if (bBreakCameraRuntimeTask)
	{
		if constexpr (std::same_as<GameplayTaskType, UGT_InitializeSceneBase>)
		{
		}
		else
		{
			const auto TempKnownTasks = KnownTasks;
			for (auto Iter : TempKnownTasks)
			{
				if (Iter && Iter->IsA(UGT_CameraTransform::StaticClass()))
				{
					Iter->EndTask();
				}
			}
		}
	}

	if (KnownTasks.IsEmpty())
	{
	}
	else
	{
	}

	auto GameplayTaskPtr = UGameplayTask::NewTask<GameplayTaskType>(
	                                                                TScriptInterface<
		                                                                IGameplayTaskOwnerInterface>(
		                                                                 this
		                                                                )
	                                                               );

	if (Func)
	{
		Func(GameplayTaskPtr);
	}

	GameplayTaskPtr->ReadyForActivation();

	return GameplayTaskPtr;
}

template <typename GameplayTaskType>
GameplayTaskType* UPlayerControllerGameplayTasksComponent::AddGameplayDelayTask(
	const std::function<void(GameplayTaskType*)>& Func
	)
{
	auto GameplayTaskPtr = UGameplayTask::NewTask<GameplayTaskType>(
																	TScriptInterface<
																		IGameplayTaskOwnerInterface>(
																		 this
																		)
																   );

	if (Func)
	{
		Func(GameplayTaskPtr);
	}

	DelayTasks.Add(GameplayTaskPtr);

	return GameplayTaskPtr;
}
