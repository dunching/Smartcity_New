
#include "VehicleRoad.h"

#include <Components/SplineComponent.h>

#include "CollisionDataStruct.h"
#include "Vehicle.h"
#include "Tools.h"

AVehicleRoad::AVehicleRoad(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f / 24;

	SceneComponentPtr = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponentPtr;

	SplineComponentPtr = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponentPtr->SetupAttachment(RootComponent);
}

void AVehicleRoad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	CopyOffsetSplines();
	GenerateVehicles();
#endif
}

#if WITH_EDITOR
void AVehicleRoad::CopyOffsetSplines()
{
	for (auto Iter : SplineComponentAry)
	{
		Iter->DestroyComponent();
	}
	SplineComponentAry.Empty();

	for (auto Iter : ColumMap)
	{
		auto SplinePtr = Cast<USplineComponent>(AddComponentByClass(
			USplineComponent::StaticClass(),
			false,
			FTransform::Identity,
			false
		));
		SplinePtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		AddInstanceComponent(SplinePtr);
		SplinePtr->ClearSplinePoints();
		if (SplineComponentPtr)
		{
			const auto Num = SplineComponentPtr->GetNumberOfSplinePoints();
			for (int32 Index = 0; Index < Num; Index++)
			{
				const auto Transform = SplineComponentPtr->GetTransformAtSplinePoint(Index, ESplineCoordinateSpace::Local);
				const auto Type = SplineComponentPtr->GetSplinePointType(Index);

#if WITH_EDITOR
				DrawDebugLine(
					GetWorld(),
					Transform.GetLocation(),
					Transform.GetLocation() + (Transform.GetRotation().GetAxisY() * Iter.YAxisOffset),
					FColor::Red,
					false,
					5
				);
#endif
				FSplinePoint SplinePoint(
					Index,
					Transform.GetLocation() + (Transform.GetRotation().GetAxisY() * Iter.YAxisOffset),
					Type
				);
				SplinePtr->AddPoint(SplinePoint, false);
			}
		}
		SplinePtr->SetClosedLoop(SplineComponentPtr->IsClosedLoop());
		SplineComponentAry.Add(SplinePtr);
	}
}

bool AVehicleRoad::Modify(bool bAlwaysMarkDirty /*= true*/)
{
	return Super::Modify(bAlwaysMarkDirty);
}
#endif

void AVehicleRoad::BeginPlay()
{
	Super::BeginPlay();

#if !WITH_EDITOR
#endif
	GenerateVehicles();
}

void AVehicleRoad::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (auto& Iter : VehiclesAry)
	{
		const auto Length = Iter.SplineComponentPtr->GetSplineLength();
		for (auto& SecondIter : Iter.VehicleStateMap)
		{
			const auto NewLen = SecondIter.Key + Iter.Speed;
			if (NewLen > Length)
			{
				SecondIter.Key = NewLen - Length;
			}
			else
			{
				SecondIter.Key = NewLen;
			}
			const auto SplineTransform = Iter.SplineComponentPtr->GetTransformAtDistanceAlongSpline(SecondIter.Key, ESplineCoordinateSpace::World);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_StreetObj);

			FHitResult OutHit;
			if (GetWorld()->LineTraceSingleByObjectType(
				OutHit,
				SplineTransform.GetLocation(),
				SplineTransform.GetLocation() + (FVector::DownVector * TraceLength),
				ObjectQueryParams
			))
			{
				const auto Pt = SplineTransform.GetLocation();
				const auto Rot = SplineTransform.GetRotation();
				SecondIter.Value->SetActorLocation(OutHit.ImpactPoint);
				SecondIter.Value->SetActorRotation(Rot);
			}
			else
			{
				const auto Pt = SplineTransform.GetLocation();
				const auto Rot = SplineTransform.GetRotation();
				SecondIter.Value->SetActorLocation(Pt);
				SecondIter.Value->SetActorRotation(Rot);
			}
		}
	}
}

void AVehicleRoad::GenerateVehicles()
{
	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	for (auto Iter : OutActors)
	{
		auto LightPtr = Cast<AVehicle>(Iter);
		if (LightPtr)
		{
			LightPtr->Destroy();
		}
	}

	for (auto Iter : VehiclesAry)
	{
		for (auto SecondIter : Iter.VehicleStateMap)
		{
			if (SecondIter.Value)
			{
				SecondIter.Value->Destroy();
			}
		}
	}
	VehiclesAry.Empty();

	if (VehicleClassArray.Num() > 0)
	{
		const auto XAxisOffset = InteravelLength / (ColumMap.Num() + 1);
		uint8 ColumIndex = 0;
		auto GenerateVehiclesBySpline = [&](USplineComponent* SplinePtr, float Speed)
		{
			const auto Length = SplinePtr->GetSplineLength();
			auto& Ref = VehiclesAry.AddDefaulted_GetRef();
			Ref.Speed = Speed;
			Ref.SplineComponentPtr = SplinePtr;
			for (float CurrentLength = ColumIndex * XAxisOffset; CurrentLength < Length; CurrentLength += InteravelLength)
			{
				const auto SplineTransform = SplinePtr->GetTransformAtDistanceAlongSpline(CurrentLength, ESplineCoordinateSpace::World);

				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;
				auto VehiclePtr = GetWorldImp()->SpawnActor<AVehicle>(
					VehicleClassArray[FMath::RandRange(0, VehicleClassArray.Num() - 1)],
					SplineTransform,
					SpawnParameters
				);
				if (VehiclePtr)
				{
					VehiclePtr->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					VehiclePtr->SetActorTransform(SplineTransform);
					Ref.VehicleStateMap.Add(CurrentLength, VehiclePtr);
				}
			}

			ColumIndex++;
		};
		if (SplineComponentPtr)
		{
			GenerateVehiclesBySpline(SplineComponentPtr, Speed);
		}
		if (SplineComponentAry.Num() == ColumMap.Num())
		{
			for (int32 Index = 0; Index < SplineComponentAry.Num(); Index++)
			{
				GenerateVehiclesBySpline(SplineComponentAry[Index], ColumMap[Index].Speed);
			}
		}
	}
}

void AVehicleRoad::Destroyed()
{
	for (const auto& Iter : VehiclesAry)
	{
		for (auto SecondIter : Iter.VehicleStateMap)
		{
			if (SecondIter.Value)
			{
				SecondIter.Value->Destroy();
			}
		}
	}
	VehiclesAry.Empty();

	Super::Destroyed();
}
