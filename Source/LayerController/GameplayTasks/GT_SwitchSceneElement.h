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

#include "GT_SwitchSceneElement.generated.h"

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
