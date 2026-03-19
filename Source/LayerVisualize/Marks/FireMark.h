// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Engine/World.h"

#include "FireMark.generated.h"

class UNiagaraComponent;
class USceneComponent;

/*
 * 用于表示火灾的标记点
 */
UCLASS()
class LAYERVISUALIZE_API AFireMark : public AActor
{
	GENERATED_BODY()

public:
	AFireMark(
		const FObjectInitializer& ObjectInitializer
		);

	void Update(
		const FVector& NewLocation
		);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
};

/*
 * 用于表示警告的标记点
 */
UCLASS()
class LAYERVISUALIZE_API AWarningMark : public AActor
{
	GENERATED_BODY()

public:
	AWarningMark(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void Tick(
		float DeltaTime
		) override;

	void Update(
		const FVector& NewLocation
		);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> AnchorComponentPtr = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> NiagaraComponent = nullptr;
};
