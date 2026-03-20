#include "GT_Camera.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameOptions.h"
#include "SceneInteractionWorldSystem.h"
#include "Algorithm.h"
#include "AssetRefMap.h"
#include "SceneElementBase.h"
#include "ViewerPawnBase.h"

void UGT_ReplyCameraTransform::Activate()
{
	Super::Activate();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(
	                                      this,
	                                      AViewerPawnBase::StaticClass(),
	                                      OutActors
	                                     );

	for (auto ActorIter : OutActors)
	{
		auto ViewerPawnPtr = Cast<AViewerPawnBase>(ActorIter);
		if (ViewerPawnPtr)
		{
			if (ViewerPawnPtr->SeatTag == SeatTag)
			{
				TargetLocation = ViewerPawnPtr->GetActorLocation();
				TargetRotation = ViewerPawnPtr->GetActorRotation();
				TargetTargetArmLength = ViewerPawnPtr->SpringArmComponent->TargetArmLength;

				return;
			}
		}
	}

	EndTask();
}

void UGT_ReplyCameraTransform::OnDestroy(
	bool bInOwnerFinished
	)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UGT_CameraTransformByPawnViewer::Activate()
{
	Super::Activate();

	if (ViewerPawnPtr)
	{
		TargetLocation = ViewerPawnPtr->GetActorLocation();
		TargetRotation = ViewerPawnPtr->GetActorRotation();
		TargetTargetArmLength = ViewerPawnPtr->SpringArmComponent->TargetArmLength;

		return;
	}

	EndTask();
}

void UGT_CameraTransformByPawnViewer::OnDestroy(
	bool bInOwnerFinished
	)
{
	Super::OnDestroy(bInOwnerFinished);
}

UGT_CameraTransform::UGT_CameraTransform(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsPausable = true;
}

void UGT_CameraTransform::Activate()
{
	Super::Activate();

	EndTask();
}

void UGT_CameraTransform::TickTask(
	float DeltaTime
	)
{
	Super::TickTask(DeltaTime);

	if (CurrentTime < Duration)
	{
		CurrentTime += DeltaTime;

		const auto Percent = CurrentTime / Duration;
		Adjust(FMath::Clamp(Percent, 0.f, 1.f));
	}
	else
	{
		Adjust(1.f);
		EndTask();
	}
}

void UGT_CameraTransform::Adjust(
	float Percent
	) const
{
}

void UGT_ModifyCameraTransform::Activate()
{
	Super::Activate();

	if (TargetTargetArmLength > 10.f)
	{
		return;
	}

	EndTask();
}

void UGT_ModifyCameraTransform::OnDestroy(
	bool bInOwnerFinished
	)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UGT_CameraTransformLocaterByID::Activate()
{
	Super::Activate();

	if (TargetDevicePtr.IsValid())
	{
	}
	else
	{
		TargetDevicePtr = USceneInteractionWorldSystem::GetInstance()->FindSceneActor(ID).Get();
	}

	if (!TargetDevicePtr.IsValid())
	{
		return;
	}

	auto Result = TargetDevicePtr->GetViewSeat();

	TargetLocation = Result.Key.GetLocation();
	TargetRotation = Result.Key.GetRotation().Rotator();
	TargetTargetArmLength = Result.Value + 500;
}

void UGT_CameraTransformLocaterBySpace::Activate()
{
	Super::Activate();

	auto TargetPtr = SpaceActorPtr;
	if (TargetPtr.IsValid())
	{
		auto Result = UKismetAlgorithm::GetCameraSeat(
		                                              {TargetPtr.Get()},
		                                              UGameOptions::GetInstance()->ViewSpaceRot,
		                                              UGameOptions::GetInstance()->ViewSpaceControlParam.FOV
		                                             );

		TargetLocation = Result.Key.GetLocation();
		TargetRotation = Result.Key.GetRotation().Rotator();
		TargetTargetArmLength = UGameOptions::GetInstance()->ViewSpaceArmLen;
	}
}
