// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "GameOptions.h"
#include "SceneElementBase.h"
#include "SceneElementManaggerBase.h"
#include "SceneElement_DeviceBase.h"
#include "GenerateTypes.h"

#include "SceneElementManagger_AccessControl.generated.h"

class UPlayerComponent;
class USphereComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class AViewerPawn;
class UActorSequenceComponent;

/**
 * 门禁
 */
UCLASS()
class LAYERVISUALIZE_API ASceneElementManagger_AccessControl :
	public ASceneElementManaggerBase
{
	GENERATED_BODY()

public:
	ASceneElementManagger_AccessControl(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Merge(
		const TSoftObjectPtr<AActor>& ActorRef,
		const TPair<FName, FString>& InUserData,
		const TMap<FName, FString>& NewUserData
		) override;

	virtual TArray<TObjectPtr<UGameplayTaskBase> > SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		) override;

	virtual void UpdateExtensionParamMap(
		const TMap<FString, FString>& NewExtensionParamMap,
		bool bImmediatelyUpdate
		) override;


	/**
	 *	Key		组
	 *	Value	固定的网格体
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> ChestMeshComponent;

	/**
	 *	Key		组
	 *	Value	旋转的网格体
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> FanMeshComponent;

	/**
	 *	Key		组
	 *	Value	旋转的锚点,左or右
	 */
	TMap<FSceneElementManaggerKeyHelper, TArray<TPair<TObjectPtr<USceneComponent>, bool>>> FanAnchorhComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RotSpeed = 90.f;

private:
	void RotationImp();

	FTimerHandle RotTimersMap;

	TMap<FSceneElementManaggerKeyHelper, float> TargetRotatorsMap;
};
