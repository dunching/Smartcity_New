#include "SceneElement_Space_PolygonHelper.h"

#include "Components/SplineMeshComponent.h"

#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

void ASceneElement_Space_PolygonHelper::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall)
		)
		{
			QuitAllState();

			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_View)
		)
		{
			// EntryFocusDevice(ConditionalSet);
			QuitAllState();
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_Focus)
		)
		{
			// EntryFocusDevice(ConditionalSet);
			QuitAllState();
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Interaction) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger)
		)
		{
			// EntryShoweviceEffect(ConditionalSet);
			QuitAllState();
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor)  &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger)
		)
		{
			// EntryShoweviceEffect(ConditionalSet);
			QuitAllState();

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
			 ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			QuitAllState();

			return;
		}
	}
	{
		if (
			(ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
			 ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Interaction)
		)
		{
			QuitAllState();

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

void ASceneElement_Space_PolygonHelper::EntryFocusDevice(
	const FSceneElementConditional& ConditionalSet
	)
{
}

void ASceneElement_Space_PolygonHelper::EntryShowevice(
	const FSceneElementConditional& ConditionalSet
	)
{
	SetActorHiddenInGame(false);

	SetColor(NormalColor);
}

void ASceneElement_Space_PolygonHelper::EntryShoweviceEffect(
	const FSceneElementConditional& ConditionalSet
	)
{
}

void ASceneElement_Space_PolygonHelper::QuitAllState()
{
	Super::QuitAllState();
	SetActorHiddenInGame(true);
}

void ASceneElement_Space_PolygonHelper::SetColor(
	const FLinearColor& Color
	)
{
	TArray<USplineMeshComponent*> Components;
	GetComponents<USplineMeshComponent>(Components);

	auto MaterialInstDynamicPtr = UMaterialInstanceDynamic::Create(MaterialInstance.LoadSynchronous(), this);

	MaterialInstDynamicPtr->SetVectorParameterValue(TEXT("Color"), Color);

	for (auto Iter : Components)
	{
		for (int32 Index = 0; Index < Iter->GetMaterials().Num(); Index++)
		{
			Iter->SetMaterial(Index, MaterialInstDynamicPtr);
		}
	}
}
