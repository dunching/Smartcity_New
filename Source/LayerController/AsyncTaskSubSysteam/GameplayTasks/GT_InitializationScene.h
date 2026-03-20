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

#include "GT_InitializationScene.generated.h"

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
