#include "SceneElement_Building_PolygonHelper.h"

#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

void ASceneElement_Building_PolygonHelper::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall)
		)
		{
			EntryShowEffect(ConditionalSet);

			return;
		}
	}
	{
		if (ConditionalSet.ConditionalSet.IsEmpty())
		{
		}

		QuitAllState();

		return;
	}
}

void ASceneElement_Building_PolygonHelper::EntryFocusDevice(
	const FSceneElementConditional& ConditionalSet
	)
{
}

void ASceneElement_Building_PolygonHelper::EntryShowevice(
	const FSceneElementConditional& ConditionalSet
	)
{
	SetActorHiddenInGame(false);
}

void ASceneElement_Building_PolygonHelper::EntryShowEffect(
	const FSceneElementConditional& ConditionalSet
	)
{
	SetActorHiddenInGame(false);
}

void ASceneElement_Building_PolygonHelper::QuitAllState()
{
	Super::QuitAllState();
	SetActorHiddenInGame(true);
}
