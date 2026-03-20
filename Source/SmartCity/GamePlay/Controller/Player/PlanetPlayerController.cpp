#include "PlanetPlayerController.h"

#include "PlayerGameplayTasks.h"
#include "PixelStreamingInputComponent.h"
#include "SmartCitySuiteTags.h"
#include "WebChannelWorldSystem.h"

APlanetPlayerController::APlanetPlayerController(const FObjectInitializer& ObjectInitializer) :
                                                                                              Super(ObjectInitializer)
{
}

void APlanetPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bool bIsNewPawn = (InPawn != GetPawn());

	if (bIsNewPawn)
	{
	}
}
