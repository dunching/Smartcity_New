
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
