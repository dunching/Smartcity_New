#include "PlanetWorldSettings.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"


#include "DataTableCollection.h"
#include "AssetRefMap.h"

void APlanetWorldSettings::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlanetWorldSettings::BeginPlay()
{
	Super::BeginPlay();
}

UAssetRefMap* APlanetWorldSettings::GetAssetRefMapInstance() const
{
	return AssetRefMapRef.LoadSynchronous();
}

UDataTableCollection* APlanetWorldSettings::GetSceneProxyExtendInfoMap() const
{
	return SceneProxyExtendInfoMapPtr.LoadSynchronous();
}