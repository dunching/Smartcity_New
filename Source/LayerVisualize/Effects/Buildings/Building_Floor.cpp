#include "Building_Floor.h"

#include "AssetRefMap.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

void ABuilding_Floor::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);


	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall)&&
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_ViewElevator)
			)
		{
			SetActorHiddenInGame(true);
			// SetActorHiddenInGame(false);
			//
			// TArray<UStaticMeshComponent*> Components;
			// GetComponents<UStaticMeshComponent>(Components);
			//
			// SetTranslucentImp(
			// 				  Components,
			// 				  80,
			// 				  UAssetRefMap::GetInstance()->ViewElevatorPostMatInst.LoadSynchronous()
			// 				 );
			//
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) ||
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			SwitchState(EState::kOriginal);

			return;
		}
	}
	{
		//  只要是楼层就显示
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
			 ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{

			SwitchState(EState::kOriginal);

			return;
		}
	}
	{
		auto EmptyContainer = FGameplayTagContainer::EmptyContainer;

		if (ConditionalSet.ConditionalSet.HasAll(EmptyContainer) && ConditionalSet.ConditionalSet.Num() ==
			EmptyContainer.Num())
		{
			SwitchState(EState::kHiden);

			return;
		}
	}
}