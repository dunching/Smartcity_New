#include "PlayerControllerBase.h"

#include "PixelStreamingInputComponent.h"

#include "GameplayTaskHelper.h"
#include "GTComponent.h"

APlayerControllerBase::APlayerControllerBase(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	GameplayTasksComponentPtr = CreateDefaultSubobject<UGTComponent>(
		UGTComponent::ComponentName
	);

	PixelStreamingInputPtr = CreateDefaultSubobject<UPixelStreamingInput>(
		TEXT("PixelStreamingInput")
	);
}

void APlayerControllerBase::OnPossess(
	APawn* InPawn
	)
{
	Super::OnPossess(InPawn);

	bool bIsNewPawn = (InPawn != GetPawn());

	if (bIsNewPawn)
	{
	}
}
