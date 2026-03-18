// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Tools.h"

bool GIsExiting = true;

UWorld* GetWorldImp()
{
#if WITH_EDITOR
	if (GEngine && GEditor && GEditor->IsPlayingSessionInEditor() && !GIsExiting)
	{
		auto Ptr = GWorld;
		return GEngine->GetCurrentPlayWorld() ? GEngine->GetCurrentPlayWorld() : GEditor->PlayWorld.Get();
	}
	else if (GEngine)
	{
		auto Ptr = GWorld;
		return GWorld ? GWorld : GEditor->PlayWorld;
	}
	else
	{
		return nullptr;
	}
#else 
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (WorldContext.World() && WorldContext.WorldType == EWorldType::Game)
		{
			return WorldContext.World();
		}
	}
	return GEngine->GetCurrentPlayWorld();
#endif
}
