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
class GAMEPLAYTASKHELPER_API UGTComponentBase : public UGameplayTasksComponent
{
	GENERATED_BODY()

public:
	static FName ComponentName;

	virtual void OnGameplayTaskDeactivated(
		UGameplayTask& Task
		) override;

protected:
	
	UPROPERTY(transient)
	TArray<TObjectPtr<UGameplayTask>> DelayTasks;

};

UCLASS()
class GAMEPLAYTASKHELPER_API UGameplayTaskBase : public UGameplayTask
{
	GENERATED_BODY()

public:
	class UGTComponentBase;

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

UCLASS()
class GAMEPLAYTASKHELPER_API UGT_RuntimeTask_Generic : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
	
	TFunction<void()> Func;
	
};
