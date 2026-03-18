// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "VehicleRoad.generated.h"

class USplineComponent;
class USceneComponent;
class UInstancedStaticMeshComponent;

class AVehicle;

USTRUCT(BlueprintType)
struct FVehicleOffset
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float YAxisOffset = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Speed = 10.f;
};

struct FVehiclesStatePerSpline
{
	USplineComponent* SplineComponentPtr = nullptr;

	TMap<float, AVehicle*>VehicleStateMap;

	float Speed = 0.f;
};

UCLASS()
class SMARTCITYSUITE_API AVehicleRoad : public AActor
{
	GENERATED_BODY()

public:

	AVehicleRoad(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform)override;

	virtual void BeginPlay()override;

	virtual void Destroyed()override;

	virtual void Tick(float DeltaSeconds)override;

protected:

#if WITH_EDITOR
	void CopyOffsetSplines();

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponentPtr = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USplineComponent* SplineComponentPtr = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<USplineComponent*> SplineComponentAry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteravelLength = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AVehicle>> VehicleClassArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceLength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVehicleOffset> ColumMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 10.f;

private:

	void GenerateVehicles();

	TArray<FVehiclesStatePerSpline> VehiclesAry;

};
