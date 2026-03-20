
#include "IPSSI.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"

UInputProcessorSubSystem_Imp* UInputProcessorSubSystem_Imp::GetInstance()
{
	return Cast<UInputProcessorSubSystem_Imp>(
									 USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
										  GetWorldImp(),
										  UInputProcessorSubSystem_Imp::StaticClass()
										 )
									);
}
