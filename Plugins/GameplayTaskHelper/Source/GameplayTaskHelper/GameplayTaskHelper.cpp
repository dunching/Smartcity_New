#include "GameplayTaskHelper.h"

#include "Engine/OverlapResult.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshActor.h"

FName UGTComponentBase::ComponentName = TEXT("PlayerControllerGameplayTasksComponent");

void UGTComponentBase::OnGameplayTaskDeactivated(
	UGameplayTask& Task
	)
{
	Super::OnGameplayTaskDeactivated(Task);

	auto GTPtr = Cast<UGameplayTaskBase>(&Task);
	if (GTPtr )
	{
		for (auto & Iter : GTPtr->NextTaskAry)
		{
			if (Iter)
			{
				Iter->ReadyForActivation();
				Iter = nullptr;
			}
		}
	}
}

void UGameplayTaskBase::OnDestroy(
	bool bInOwnerFinished
	)
{
	OnTaskComplete.Broadcast(true);

	Super::OnDestroy(bInOwnerFinished);
}

UGT_BatchStepBase::UGT_BatchStepBase(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	bTickingTask = true;
}

void UGT_BatchStepBase::TickTask(
	float DeltaTime
	)
{
	Super::TickTask(DeltaTime);

	switch (UseScopeType)
	{
	case EUseScopeType::kTime:
		{
			double TotalTime = 0.;
			for (;;)
			{
				double InScopeSeconds = 0.;
				{
					FSimpleScopeSecondsCounter SimpleScopeSecondsCounter(InScopeSeconds);

					if (ProcessTask(DeltaTime))
					{
					}
					else
					{
						break;
					}
				}
				TotalTime += InScopeSeconds;

				if (TotalTime > ScopeTime)
				{
					return;
				}

				if (UseScopeType != EUseScopeType::kTime)
				{
					return;
				}
			}
		}
		break;
	case EUseScopeType::kCount:
		{
			// 
			//
			//
			// ON_SCOPE_EXIT
			// {
			// 	if (TotalTime > 0.3)
			// 	{
			// 		// checkNoEntry();
			// 	}
			// 	PRINTINVOKEWITHSTR(FString::Printf(TEXT("TotalTime %.2lf"), TotalTime));
			// };

			int32 CurrentTickProcessNum = 0;
			for (;;)
			{
				if (ProcessTask(DeltaTime))
				{
					CurrentTickProcessNum++;
					if (CurrentTickProcessNum < PerTickProcessNum)
					{
					}
					else
					{
						CurrentTickProcessNum = 0;
						return;
					}

					if (UseScopeType != EUseScopeType::kCount)
					{
						return;
					}
				}
				else
				{
					break;
				}
			}
		}
		break;
	case EUseScopeType::kNone:
		{
			if (ProcessTask(DeltaTime))
			{
				return;
			}
			else
			{
				break;
			}
		}
		break;
	}

	EndTask();
}

bool UGT_BatchStepBase::ProcessTask(
	float DeltaTime
	)
{
	return false;
}
