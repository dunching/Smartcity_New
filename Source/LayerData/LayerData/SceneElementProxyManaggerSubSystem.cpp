
#include "SceneElementProxyManaggerSubSystem.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"

#include "Tools.h"

 USceneElementProxyManaggerSubSystem* USceneElementProxyManaggerSubSystem::GetInstance()
 {
  return Cast<USceneElementProxyManaggerSubSystem>(
                                         USubsystemBlueprintLibrary::GetWorldSubsystem(
                                           GetWorldImp(),
                                           USceneElementProxyManaggerSubSystem::StaticClass()
                                          )
                                        );
}
