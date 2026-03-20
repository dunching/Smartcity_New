
#include "AsyncTaskSubSysteam.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"

UAsyncTaskSubSysteam* UAsyncTaskSubSysteam::GetInstance()
{
	return Cast<UAsyncTaskSubSysteam>(
									 USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
										  GetWorldImp(),
										  UAsyncTaskSubSysteam::StaticClass()
										 )
									);
}

void FQueued_SwitchSceneElement::DoThreadedWork()
{
}

void FQueued_SwitchSceneElement::Abandon()
{
	delete this;
}
