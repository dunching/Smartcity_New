// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "BuildingGenerateTypes.h"

#include "SceneElementManaggerBase.generated.h"

class USceneComponent;
class UInstancedStaticMeshComponent;
class UGameplayTaskBase;

/**
 * 按类型管理场景对象基类
 */
UCLASS()
class BUILDINGTOOLS_API ASceneElementManaggerBase :
	public AActor
{
	GENERATED_BODY()

public:
	ASceneElementManaggerBase(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void InitialSceneElement();

	virtual void InitialEndSceneElement();

	/**
	 * 合并所有
	 * @param ActorRef
	 * @param InUserData
	 * @param NewUserData 
	 */
	virtual void Merge(
		const TSoftObjectPtr<AActor>& ActorRef,
		const TPair<FName, FString>& InUserData,
		const TMap<FName, FString>& NewUserData
		);

	void UpdateInteractionType(
		const FSceneElementConditional& ConditionalSet
		);

	virtual TArray<TObjectPtr<UGameplayTaskBase> > SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		);

	virtual void UpdateExtensionParamMap(
		const TMap<FString, FString>& NewExtensionParamMap,
		bool bImmediatelyUpdate
		);

	/**
	 * 退出所有状态
	 */
	virtual void QuitAllState(
		);

	/**
	 * Bim ID
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString SceneElementID;


	/**
	 *	
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> ISMAry;

	/**
	 * 楼层
	 * Index
	 */
	TMap<int32, TArray<FModelIndex>>FloorIndexMap;
	
	TArray<FString>TempIndex;
	
	FSceneElementConditional CurrentConditionalSet;

	TMap<FName, FString> UserData;
};
