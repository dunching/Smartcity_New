// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "GameplayTasksComponent.h"

#include "GameplayTaskHelper.h"
#include "GT_Camera.h"
#include "GT_InitializationScene.h"

#include "GTComponent.generated.h"

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
class LAYERCONTROLLER_API UGTComponent : public UGTComponentBase
{
	GENERATED_BODY()

public:
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
	
};

template <typename GameplayTaskType>
GameplayTaskType* UGTComponent::StartGameplayTask(
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
GameplayTaskType* UGTComponent::AddGameplayDelayTask(
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
