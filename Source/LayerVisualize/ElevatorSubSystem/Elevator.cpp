#include "Elevator.h"

#include "ActorSequenceComponent.h"
#include "AssetRefMap.h"

#include "CollisionDataStruct.h"
#include "ElevatorMark.h"
#include "FloorHelper.h"
#include "SmartCitySuiteTags.h"

AElevator::AElevator(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	UpdateElevatorDisplay();
}

void AElevator::ReplaceImp(
	AActor* ActorPtr,
	const TPair<FName, FString>& InUserData
	)
{
	Super::ReplaceImp(ActorPtr, InUserData);
}

void AElevator::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) &&
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_ViewElevator)
		)
		{
			SetActorHiddenInGame(false);

			ElevatorMarkRef.LoadSynchronous()->SetActorHiddenInGame(false);

			TArray<UStaticMeshComponent*> StaticMeshComponents;
			GetComponents<UStaticMeshComponent>(StaticMeshComponents);

			for (auto Iter : StaticMeshComponents)
			{
				Iter->SetRenderCustomDepth(true);
				Iter->SetCustomDepthStencilValue(1);
			}

#if TEST_ELEVATOR
			ChangeTargetFloorIndex(FMath::RandRange(LowFloor, HighFloor));
#endif
			
			return;
		}
	}

	{
		if (ConditionalSet.ConditionalSet.IsEmpty())
		{
		}

		QuitAllState();

		return;
	}
}

void AElevator::ChangeTargetFloorIndex(
	int32 FloorIndex
	)
{
	if (TargetFloorIndex != FloorIndex)
	{
		TargetFloorIndex = FloorIndex;

		for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
		{
			if (!Iter.Value->bIsSup && (Iter.Value->FloorIndex == FloorIndex))
			{
				TargetFloorLocation = Iter.Value->GetActorLocation();

				const auto CurrentLocation = GetActorLocation();

				bIsUp = CurrentLocation.Z < TargetFloorLocation.Z;

				GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ThisClass::MoveElevator, Frequence, true);

				return;;
			}
		}
	}
	
#if TEST_ELEVATOR
	ChangeTargetFloorIndex(FMath::RandRange(LowFloor, HighFloor));
#endif
			
}

void AElevator::QuitAllState()
{
	Super::QuitAllState();

	ElevatorMarkRef.LoadSynchronous()->SetActorHiddenInGame(true);

	SetActorHiddenInGame(true);

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (auto Iter : StaticMeshComponents)
	{
		Iter->SetRenderCustomDepth(false);
	}
	
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
}

void AElevator::MoveElevator()
{
	if (bIsUp)
	{
		const auto CurrentLocation = GetActorLocation();

		const auto NewLocation = CurrentLocation + (FVector::UpVector * Frequence * MoveSpeed);

		if (NewLocation.Z < TargetFloorLocation.Z)
		{
			SetActorLocation(NewLocation);
		}
		else
		{
			SetActorLocation(FVector(NewLocation.X, NewLocation.Y, TargetFloorLocation.Z));
			GetWorldTimerManager().ClearTimer(MoveTimerHandle);

			
#if TEST_ELEVATOR
			ChangeTargetFloorIndex(FMath::RandRange(LowFloor, HighFloor));
#endif
			
		}
	}
	else
	{
		const auto CurrentLocation = GetActorLocation();

		const auto NewLocation = CurrentLocation + (FVector::DownVector * Frequence * MoveSpeed);
		if (NewLocation.Z > TargetFloorLocation.Z)
		{
			SetActorLocation(NewLocation);
		}
		else
		{
			SetActorLocation(FVector(NewLocation.X, NewLocation.Y, TargetFloorLocation.Z));
			GetWorldTimerManager().ClearTimer(MoveTimerHandle);

			
#if TEST_ELEVATOR
			ChangeTargetFloorIndex(FMath::RandRange(LowFloor, HighFloor));
#endif
			
		}
	}

	UpdateElevatorDisplay();
}

void AElevator::UpdateElevatorDisplay()
{
	auto NewLocation = GetActorLocation();

	int32 CurrentIndex = -100;
	int32 MinDis = -1;
	for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (!Iter.Value->bIsSup)
		{
			const auto FloorLocation = Iter.Value->GetActorLocation();

			if (MinDis < 0)
			{
				MinDis = FMath::Abs(NewLocation.Z - FloorLocation.Z);
				CurrentIndex = Iter.Value->FloorIndex;
			}
			else
			{
				const auto Dis = FMath::Abs(NewLocation.Z - FloorLocation.Z);
				if (Dis < MinDis)
				{
					MinDis = Dis;
					CurrentIndex = Iter.Value->FloorIndex;
				}
			}
		}
	}

	ElevatorMarkRef.LoadSynchronous()->UpdateFloor(CurrentIndex);
	ElevatorMarkRef.LoadSynchronous()->SetActorLocation(GetActorLocation());

}
