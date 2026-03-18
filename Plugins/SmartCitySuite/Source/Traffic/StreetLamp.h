// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "StreetLamp.generated.h"

class USplineComponent;
class USceneComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class SMARTCITYSUITE_API AStreetLamp : public AActor
{
	GENERATED_BODY()

public:

	AStreetLamp();

	AStreetLamp(const FObjectInitializer& ObjectInitializer);

	virtual ~AStreetLamp();

	virtual void OnConstruction(const FTransform& Transform)override;

	virtual void BeginPlay()override;

	virtual void BeginDestroy() override;

	virtual void Destroyed()override;

	void OnHoury(int32 Hour);

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSoftObjectPtr<UCurveFloat>LightEnableStateCurve;

	UPROPERTY(Category = Pawn, BlueprintReadWrite, EditAnywhere)
		USceneComponent* ScenceComponentPtr = nullptr;

	int32 EventID = 0;

};

UCLASS()
class SMARTCITYSUITE_API AStreetLampHelper : public AActor
{
	GENERATED_BODY()
public:

	AStreetLampHelper(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform)override;

	void GenerateLamps();

	virtual void BeginPlay()override;

	virtual void Destroyed()override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USplineComponent* SplineComponentPtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StepDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStreetLamp> StreetLampClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceLength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsTarceLand = true;

protected:

	TArray<AStreetLamp*>LightAry;

};
