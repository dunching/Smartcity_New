// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>
#include <map>
#include <set>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "GameplayTasksComponent.h"
#include "GenerateTypes.h"
#include "LogWriter.h"
#include "SceneInteractionWorldSystem.h"
#include "GameplayTaskHelper.h"

#include "GT_Camera.generated.h"

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

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_CameraTransform : public UGameplayTaskBase
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	UGT_CameraTransform(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Activate() override;

	virtual void TickTask(
		float DeltaTime
		) override;

	float Duration = 1.f;

	FVector TargetLocation;

	FRotator TargetRotation;

	float TargetTargetArmLength = 0.f;

protected:
	void Adjust(
		float Percent
		) const;

private:
	float CurrentTime = 0.f;

	FVector OriginalLocation;

	FRotator OriginalRotation;

	float OriginalSpringArmLen = 0.f;
};

/**
 * 恢复到原始视角
 */
UCLASS()
class LAYERCONTROLLER_API UGT_ReplyCameraTransform : public UGT_CameraTransform
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	virtual void Activate() override;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;

	FGameplayTag SeatTag;
};

UCLASS()
class LAYERCONTROLLER_API UGT_CameraTransformByPawnViewer : public UGT_CameraTransform
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	virtual void Activate() override;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;

	TObjectPtr<AViewerPawnBase> ViewerPawnPtr = nullptr;
};

/**
 * 移动到指定Transform
 */
UCLASS()
class LAYERCONTROLLER_API UGT_ModifyCameraTransform : public UGT_CameraTransform
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	virtual void Activate() override;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;
};

/**
 * 定位某个设备
 */
UCLASS()
class LAYERCONTROLLER_API UGT_CameraTransformLocaterByID : public UGT_CameraTransform
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	virtual void Activate() override;

	FString ID;

	TWeakObjectPtr<ASceneElementBase> TargetDevicePtr = nullptr;
};

/**
 * 定位某个区域
 */
UCLASS()
class LAYERCONTROLLER_API UGT_CameraTransformLocaterBySpace : public UGT_CameraTransform
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	virtual void Activate() override;

	TWeakObjectPtr<AActor> SpaceActorPtr;
};
