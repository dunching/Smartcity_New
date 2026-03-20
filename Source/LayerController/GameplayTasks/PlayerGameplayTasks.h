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

#include "PlayerGameplayTasks.generated.h"

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

#pragma region 摄像机修改

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

#pragma endregion

#pragma region 数据处理

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_InitializeSceneActors : public UGT_InitializeSceneBase
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool
		)>;

	UGT_InitializeSceneActors(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Activate() override;

	virtual void TickTask(
		float DeltaTime
		) override;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;

	USceneInteractionWorldSystem* SceneInteractionWorldSystemPtr = nullptr;

	FOnEnd OnEnd;

	enum class EStep
	{
		kFloor_ControlBorder,
		kFloor_AS,
		kFloor_ID,
		kFloor_Sost,
		kFloor_Space,
		kBuilding,
		kLandscape,
		kComplete,
	};

	EStep Step = EStep::kFloor_ControlBorder;

protected:
	virtual bool ProcessTask(
		float DeltaTime
		) override;

	void ProcessTask_ControlBorder(
		const TSoftObjectPtr<AFloorHelper>& FloorHelperRef
		);

	bool ProcessTask_StructItemSet(
		ASceneElementCategory* SceneElementCategoryPtr,
		FSceneElementMap& AllReference
		);

	bool ProcessTask_InnerStructItemSet(
		ASceneElementCategory* SceneElementCategoryPtr,
		FSceneElementMap& AllReference
		);

	bool ProcessTask_SoftDecorationItemSet(
		ASceneElementCategory* SceneElementCategoryPtr,
		FSceneElementMap& AllReference
		);

	bool ProcessTask_SpaceItemSet(
		ASceneElementCategory* SceneElementCategoryPtr,
		FSceneElementMap& AllReference
		);

	bool ProcessTask_Lanscape(
		ASceneElementCategory* SceneElementCategoryPtr,
		TSet<TSoftObjectPtr<ADatasmithSceneActor>>& DatasmithSceneActorSet
		);

private:
	bool ProcessTask_Generic(
		ASceneElementCategory* SceneElementCategoryPtr,
		FSceneElementMap& AllReference,
		FSceneElementFilter& StructItemSet
		);

	bool ProcessTask_Generic_NeedReplaceByUserData(
		UDatasmithAssetUserData* AUDPtr,
		const FString& Datasmith_UniqueId,
		AActor* ActorPtr
		);

	bool ProcessTask_Generic_NeedMergeByUserData(
		UDatasmithAssetUserData* AUDPtr,
		const FString& Datasmith_UniqueId,
		AActor* ActorPtr,
		TMap<int32, ASceneElementBase*>& MergeActorsMap
		);

	bool ProcessTask_Generic_SceneElementManaggerNeedMergeByUserData(
		UDatasmithAssetUserData* AUDPtr,
		const FString& Datasmith_UniqueId,
		AActor* ActorPtr,
		TMap<int32, ASceneElementBase*>& MergeActorsMap
		);


	TMap<int32, ASceneElementManaggerBase*> SceneElementManaggerMergeActorsMap;
};

#pragma endregion

#pragma region 场景对象切换

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Base : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
	using FOnEnd = TMulticastDelegate<void(
		bool,
		UGT_SwitchSceneElement_Base*


		
		)>;

	UGT_SwitchSceneElement_Base(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Activate() override;

	virtual void TickTask(
		float DeltaTime
		) override;

	virtual void OnDestroy(
		bool bInOwnerFinished
		) override;

	USceneInteractionWorldSystem* SceneInteractionWorldSystemPtr = nullptr;

	FSceneElementConditional FilterTags;

	/**
	 * 是否清除之前的
	 */
	bool bClearPrevious = false;

	FOnEnd OnEnd;

protected:
	virtual bool ProcessTask(
		float DeltaTime
		) override;

	virtual bool ProcessTask_Display();

	virtual bool ProcessTask_Hiden();

	virtual bool ProcessTask_ConfirmConditional();

	virtual bool ProcessTask_SwitchState();

	virtual bool ProcessTask_SwitchState_SceneElementManagger();

	virtual bool ProcessTask_SwitchState_Elevator();

	enum class EStep
	{
		kDisplay,
		kHiden,
		kConfirmConditional,
		kSwitchState,
		kSwitchState_SceneElementManagger,
		kSwitchState_Elevator,
		kComplete,
	};

	EStep Step = EStep::kSwitchState_Elevator;

protected:
	TArray<TObjectPtr<ASceneElementBase>> NeedDisplayAry;

	int32 DisplayAryIndex = 0;

	TArray<TPair<FString, ASceneElementManaggerBase*>> NeedDisplaySceneElementManaggerAry;

	int32 DisplaySceneElementManaggerAryIndex = 0;

	TArray<TObjectPtr<ASceneElementBase>> NeedHideAry;

	int32 HideAryIndex = 0;
};

UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Tower : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	bool bDisplayInnerStruct = true;

protected:
private:
	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_ConfirmConditional() override;

	virtual bool ProcessTask_SwitchState() override;

	virtual bool ProcessTask_SwitchState_Elevator() override;

	TArray<TObjectPtr<ASceneElementBase>> BuildingsAry;

	int32 BuildingsAryIndex = 0;
};

UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Floor : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	TSet<TObjectPtr<ASceneElementBase>> SkipSceneElementSet;

	TSet<FGameplayTag> FloorSet;
};

UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Floor_JF : public UGT_SwitchSceneElement_Floor
{
	GENERATED_BODY()

public:
	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_SwitchState() override;

	TArray<TObjectPtr<ASceneElementBase>> PriorityHideFloorAryAry;

	int32 PriorityHideFloorAryIndex = 0;
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Space : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_SwitchState() override;

	TSet<TObjectPtr<ASceneElementBase>> SkipSceneElementSet;

	TWeakObjectPtr<ASceneElement_Space> SceneElementPtr = nullptr;

	TSet<FGameplayTag> FloorSet;
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_Device : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_SwitchState() override;

	TSet<TObjectPtr<ASceneElement_DeviceBase>> SceneElementSet;

	TSet<FGameplayTag> FloorSet;
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_BatchDevicesControl : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_SwitchState() override;

	TSet<TObjectPtr<ASceneElement_DeviceBase>> SceneElementSet;

	FGameplayTag FloorTag;

	TMap<FString, TMap<FString, FString>> ExtensionParamMap;
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_SwitchSceneElement_SpecialArea : public UGT_SwitchSceneElement_Base
{
	GENERATED_BODY()

public:
	virtual bool ProcessTask_Display() override;

	virtual bool ProcessTask_Hiden() override;

	virtual bool ProcessTask_SwitchState() override;

	TArray<TObjectPtr<ASceneElementBase>> PriorityHideFloorAryAry;

	int32 PriorityHideFloorAryIndex = 0;

	TSet<FGameplayTag> FloorSet;

	TSet<FGameplayTag> PriorityHideFloorSet;
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_FloorSplit : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
};

/**
 * 
 */
UCLASS()
class LAYERCONTROLLER_API UGT_QuitFloorSplit : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
};

#pragma endregion

UCLASS()
class LAYERCONTROLLER_API UGT_RuntimeTask_Generic : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
	
	TFunction<void()> Func;
	
};
