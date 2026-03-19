// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "GameOptions.h"
#include "SceneElementBase.h"
#include "SceneElement_DeviceBase.h"
#include "Tools.h"

#include "Building_ItemBase.generated.h"

class UStaticMeshComponent;

/**
 * 墙
 */
UCLASS()
class LAYERVISUALIZE_API ABuilding_ItemBase :
	public ASceneElementBase
{
	GENERATED_BODY()

public:
	ABuilding_ItemBase(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void ReplaceImp(
		AActor* ActorPtr,
		const TPair<FName, FString>& InUserData
		) override;

	virtual void Merge(
		const TSoftObjectPtr<AActor>& ActorRef,
		const TPair<FName, FString>& InUserData,
		const TMap<FName, FString>& NewUserData
		) override;

	virtual void SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		) override;

	enum class EState : uint8
	{
		kOriginal,
		kHiden,
	};

	void SwitchState(
		EState State
		);

	/**
	 * 网格体
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMeshComponent*> StaticMeshComponentsAry;

	FGameplayTag Floor;
};
