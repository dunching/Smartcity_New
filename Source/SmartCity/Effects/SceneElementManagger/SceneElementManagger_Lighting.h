// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "GameOptions.h"
#include "SceneElementBase.h"
#include "SceneElementManaggerBase.h"
#include "SceneElement_DeviceBase.h"
#include "GenerateTypes.h"
#include "PlayerGameplayTasks.h"

#include "SceneElementManagger_Lighting.generated.h"

class UPlayerComponent;
class USphereComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class AViewerPawn;
class UActorSequenceComponent;
class UInstancedStaticMeshComponent;
class UGameplayTaskBase;

UCLASS()
class SMARTCITY_API UGT_SwitchSceneElementManagger_Lighting_Display : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
	virtual bool ProcessTask(
		float DeltaTime
		) override;

	TObjectPtr<ASceneElementManaggerBase>SceneElementManaggerPtr = nullptr;
	
	TArray<FModelIndex> DisplayAry;

	int32 DisplayIndex = 0;
};

UCLASS()
class SMARTCITY_API UGT_SwitchSceneElementManagger_Lighting_Hide : public UGT_RuntimeTask
{
	GENERATED_BODY()

public:
	virtual bool ProcessTask(
		float DeltaTime
		) override;

	TObjectPtr<ASceneElementManaggerBase>SceneElementManaggerPtr = nullptr;
	
	TArray<FModelIndex> HideAry;

	int32 HideIndex = 0;
};

/**
 * 门禁
 */
UCLASS()
class SMARTCITY_API ASceneElementManagger_Lighting :
	public ASceneElementManaggerBase
{
	GENERATED_BODY()

public:
	ASceneElementManagger_Lighting(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Merge(
		const TSoftObjectPtr<AActor>& ActorRef,
		const TPair<FName, FString>& InUserData,
		const TMap<FName, FString>& NewUserData
		) override;

	virtual TArray<TObjectPtr<UGameplayTaskBase>> SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		) override;

	virtual void UpdateExtensionParamMap(
		const TMap<FString, FString>& NewExtensionParamMap,
		bool bImmediatelyUpdate
		) override;

private:
	TArray<TObjectPtr<UGameplayTaskBase>> SwitchFloor(
		const FSceneElementConditional& ConditionalSet
		) const;
};
