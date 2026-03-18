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

#include "SceneElementManagger_Furniture.generated.h"

class UPlayerComponent;
class USphereComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class AViewerPawn;
class UActorSequenceComponent;
class UInstancedStaticMeshComponent;
class UGameplayTaskBase;

/**
 * 门禁
 */
UCLASS()
class SMARTCITY_API ASceneElementManagger_Furniture:
	public ASceneElementManaggerBase
{
	GENERATED_BODY()

public:
	ASceneElementManagger_Furniture(
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
