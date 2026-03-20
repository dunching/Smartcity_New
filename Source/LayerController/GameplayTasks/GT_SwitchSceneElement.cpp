#include "GT_SwitchSceneElement.h"

#include "DatasmithAssetUserData.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshActor.h"

#include "GameOptions.h"
#include "SceneInteractionWorldSystem.h"
#include "Algorithm.h"
#include "AssetRefMap.h"
#include "Building_CurtainWall.h"
#include "TowerHelperBase.h"
#include "DatasmithSceneActor.h"
#include "FloorHelper.h"
#include "GenerateTypes.h"
#include "LogWriter.h"
#include "ReplaceActor.h"
#include "SceneElementBase.h"
#include "TemplateHelper.h"
#include "CollisionDataStruct.h"
#include "Elevator.h"
#include "LandScapeBase.h"
#include "SceneElementCategory.h"
#include "SceneElementManaggerBase.h"
#include "SceneElement_ControlBorder.h"
#include "SceneElement_LandScape.h"
#include "SceneElement_PWR_Pipe.h"
#include "SceneElement_Regualar.h"
#include "SceneElement_Space.h"
#include "SmartCitySuiteTags.h"
#include "ViewerPawnBase.h"

UGT_SwitchSceneElement_Base::UGT_SwitchSceneElement_Base(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsPausable = true;

	Priority = 1.5 * FGameplayTasks::DefaultPriority;
}

void UGT_SwitchSceneElement_Base::Activate()
{
	Super::Activate();

	FilterTags.ConditionalSet.RemoveTag(USmartCitySuiteTags::Interaction_Mode_Empty);
}

void UGT_SwitchSceneElement_Base::TickTask(
	float DeltaTime
	)
{
	Super::TickTask(DeltaTime);
}

void UGT_SwitchSceneElement_Base::OnDestroy(
	bool bInOwnerFinished
	)
{
	Super::OnDestroy(bInOwnerFinished);
}

bool UGT_SwitchSceneElement_Base::ProcessTask(
	float DeltaTime
	)
{
	switch (Step)
	{
	case EStep::kSwitchState_Elevator:
		{
			if (ProcessTask_SwitchState_Elevator())
			{
				return true;
			}
			else
			{
			}

			Step = EStep::kDisplay;
			return true;
		}
	case EStep::kDisplay:
		{
			if (ProcessTask_Display())
			{
				return true;
			}
			else
			{
			}

			Step = EStep::kHiden;
			return true;
		}
		break;
	case EStep::kHiden:
		{
			if (ProcessTask_Hiden())
			{
				return true;
			}
			else
			{
			}

			Step = EStep::kConfirmConditional;
			return true;
		}
		break;
	case EStep::kConfirmConditional:
		{
			if (ProcessTask_ConfirmConditional())
			{
				return true;
			}
			else
			{
			}

			UseScopeType = EUseScopeType::kCount;
			PerTickProcessNum = 500;

			Step = EStep::kSwitchState;
			return true;
		}
		break;
	case EStep::kSwitchState:
		{
			if (ProcessTask_SwitchState())
			{
				return true;
			}
			else
			{
			}

			Step = EStep::kSwitchState_SceneElementManagger;
			return true;
		}
	case EStep::kSwitchState_SceneElementManagger:
		{
			if (ProcessTask_SwitchState_SceneElementManagger())
			{
				return true;
			}
			else
			{
			}

			Step = EStep::kComplete;
			return true;
		}
	case EStep::kComplete:
	default:
		{
		}
	}

	if (OnEnd.IsBound())
	{
		OnEnd.Broadcast(true, this);
	}

	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_Display()
{
	const auto SceneElementManaggerMap = USceneInteractionWorldSystem::GetInstance()->SceneElementManaggerMap;
	for (const auto& FloorIter : SceneElementManaggerMap)
	{
		NeedDisplaySceneElementManaggerAry.Add(FloorIter);
	}

	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_Hiden()
{
	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_ConfirmConditional()
{
	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_SwitchState()
{
	if (DisplayAryIndex < NeedDisplayAry.Num())
	{
		ON_SCOPE_EXIT
		{
			DisplayAryIndex++;
		};

		auto ActorPtr = NeedDisplayAry[DisplayAryIndex];
		if (ActorPtr)
		{
			auto SceneElementPtr = Cast<ASceneElementBase>(ActorPtr);
			if (SceneElementPtr)
			{
				// auto SceneElement_ControlBorderPtr = Cast<ASceneElement_ControlBorder>(
				// 	 SceneElementPtr->GetAttachParentActor()
				// 	);
				// if (SceneElement_ControlBorderPtr)
				// {
				// }
				// else
				// {
				SceneElementPtr->UpdateInteractionType(FilterTags);
				// }
			}
			else
			{
				ActorPtr->SetActorHiddenInGame(false);

				TArray<AActor*> OutActors;

				ActorPtr->GetAttachedActors(OutActors, true, true);
				for (auto Iter : OutActors)
				{
					Iter->SetActorHiddenInGame(false);
				}
			}
		}

		return true;
	}

	if (HideAryIndex < NeedHideAry.Num())
	{
		ON_SCOPE_EXIT
		{
			HideAryIndex++;
		};

		auto ActorPtr = NeedHideAry[HideAryIndex];
		if (ActorPtr)
		{
			auto SceneElementPtr = Cast<ASceneElementBase>(ActorPtr);
			if (SceneElementPtr)
			{
				// auto SceneElement_ControlBorderPtr = Cast<ASceneElement_ControlBorder>(
				// 	 SceneElementPtr->GetAttachParentActor()
				// 	);
				// if (SceneElement_ControlBorderPtr)
				// {
				// }
				// else
				// {
				SceneElementPtr->UpdateInteractionType(FSceneElementConditional::EmptyConditional);
				// }
			}
			else
			{
				ActorPtr->SetActorHiddenInGame(true);

				TArray<AActor*> OutActors;

				ActorPtr->GetAttachedActors(OutActors, true, true);
				for (auto Iter : OutActors)
				{
					Iter->SetActorHiddenInGame(true);
				}
			}
		}

		return true;
	}

	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_SwitchState_SceneElementManagger()
{
	if (DisplaySceneElementManaggerAryIndex < NeedDisplaySceneElementManaggerAry.Num())
	{
		ON_SCOPE_EXIT
		{
			DisplaySceneElementManaggerAryIndex++;
		};

		auto ActorPtr = NeedDisplaySceneElementManaggerAry[DisplaySceneElementManaggerAryIndex];
		if (ActorPtr.Value)
		{
			auto SceneElementPtr = Cast<ASceneElementManaggerBase>(ActorPtr.Value);
			if (SceneElementPtr)
			{
				SceneElementPtr->UpdateInteractionType(FilterTags);
			}
		}

		return true;
	}

	return false;
}

bool UGT_SwitchSceneElement_Base::ProcessTask_SwitchState_Elevator()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->ElevatorMap)
	{
		if (FloorIter.Value)
		{
			FloorIter.Value->UpdateInteractionType(FilterTags);
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Tower::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		BuildingsAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_AS))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						NeedDisplayAry.Add(SceneElementBasePtr);
					}
				}
			}
			else if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_ID))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						NeedDisplayAry.Add(SceneElementBasePtr);
					}
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->BuildingHelpers)
	{
		BuildingsAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedDisplayAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		BuildingsAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedDisplayAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Tower::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Soft))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						NeedHideAry.Add(SceneElementBasePtr);
					}
				}
			}
			else if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Space))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						NeedHideAry.Add(SceneElementBasePtr);
					}
				}
			}
		}
	}

	for (const auto& Iter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
	}

	return false;
}

bool UGT_SwitchSceneElement_Tower::ProcessTask_ConfirmConditional()
{
	return false;
}

bool UGT_SwitchSceneElement_Tower::ProcessTask_SwitchState()
{
	const auto Value = Super::ProcessTask_SwitchState();
	if (Value)
	{
		return true;
	}
	else
	{
		if (BuildingsAryIndex < BuildingsAry.Num())
		{
			ON_SCOPE_EXIT
			{
				BuildingsAryIndex++;
			};

			auto ActorPtr = BuildingsAry[BuildingsAryIndex];
			if (ActorPtr)
			{
				auto SceneElementPtr = Cast<ASceneElementBase>(ActorPtr);
				if (SceneElementPtr)
				{
					SceneElementPtr->UpdateInteractionType(FilterTags);
				}
				else
				{
					ActorPtr->SetActorHiddenInGame(false);

					TArray<AActor*> OutActors;

					ActorPtr->GetAttachedActors(OutActors, true, true);
					for (auto Iter : OutActors)
					{
						Iter->SetActorHiddenInGame(false);
					}
				}
			}

			return true;
		}
	}
	return false;
}

bool UGT_SwitchSceneElement_Tower::ProcessTask_SwitchState_Elevator()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->ElevatorMap)
	{
		if (FloorIter.Value)
		{
			FloorIter.Value->UpdateInteractionType(FilterTags);
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Floor::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
		}
		else
		{
			continue;
		}

		NeedDisplayAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					if (SkipSceneElementSet.Contains(SceneElementBasePtr))
					{
						PRINTFUNC();
					}
					else
					{
						NeedDisplayAry.Add(SceneElementBasePtr);
					}
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Floor::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Floor_JF::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorIter.Value->FloorTag.MatchesTagExact(USmartCitySuiteTags::Interaction_Area_Floor_F12))
		{
		}
		else
		{
			continue;
		}

		PriorityHideFloorAryAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					PriorityHideFloorAryAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		if (FloorIter.Value->FloorTag.MatchesTagExact(USmartCitySuiteTags::Interaction_Area_Floor_F12))
		{
			continue;
		}
		else
		{
		}

		NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Floor_JF::ProcessTask_SwitchState()
{
	if (PriorityHideFloorAryIndex < PriorityHideFloorAryAry.Num())
	{
		ON_SCOPE_EXIT
		{
			PriorityHideFloorAryIndex++;
		};

		auto ActorPtr = PriorityHideFloorAryAry[PriorityHideFloorAryIndex];
		if (ActorPtr)
		{
			auto SceneElementPtr = Cast<ASceneElementBase>(ActorPtr);
			if (SceneElementPtr)
			{
				SceneElementPtr->UpdateInteractionType(FSceneElementConditional::EmptyConditional);
			}
			else
			{
				ActorPtr->SetActorHiddenInGame(true);

				TArray<AActor*> OutActors;

				ActorPtr->GetAttachedActors(OutActors, true, true);
				for (auto Iter : OutActors)
				{
					Iter->SetActorHiddenInGame(true);
				}
			}
		}

		return true;
	}

	return Super::ProcessTask_SwitchState();
}

void UGT_SwitchSceneElement_Space::Activate()
{
	Super::Activate();

	if (SceneElementPtr.IsValid())
	{
		// FloorSet = SceneElementPtr->BelongFloor->FloorTag;
	}

	// FilterTags.ConditionalSet = FGameplayTagContainer{FloorTag};
}

bool UGT_SwitchSceneElement_Space::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
		}
		else
		{
			continue;
		}

		NeedDisplayAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Space))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						if (ActorIter == SceneElementPtr)
						{
						}
						else if (SkipSceneElementSet.Contains(SceneElementBasePtr))
						{
							PRINTFUNC();
						}
						else
						{
							NeedDisplayAry.Add(SceneElementBasePtr);
						}
					}
				}
				continue;
			}

			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					if (SkipSceneElementSet.Contains(SceneElementBasePtr))
					{
						PRINTFUNC();
					}
					else
					{
						NeedDisplayAry.Add(SceneElementBasePtr);
					}
				}
			}
		}
	}
	return false;
}

bool UGT_SwitchSceneElement_Space::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Space::ProcessTask_SwitchState()
{
	if (SceneElementPtr.IsValid())
	{
		FSceneElementConditional TempFilterTags;

		TempFilterTags.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Mode_View);

		if (SceneElementPtr.IsValid())
		{
			SceneElementPtr->UpdateInteractionType(TempFilterTags);
		}

		SceneElementPtr = nullptr;
		return true;
	}

	if (Super::ProcessTask_SwitchState())
	{
		return true;
	}

	return false;
}

void UGT_SwitchSceneElement_Device::Activate()
{
	Super::Activate();

	for (const auto& Iter : FloorSet)
	{
		FilterTags.ConditionalSet.AddTag(Iter);
	}
}

bool UGT_SwitchSceneElement_Device::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
		}
		else
		{
			continue;
		}

		NeedDisplayAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Soft))
			{
				TArray<AActor*> OutActors;

				Iter.Value->GetAttachedActors(OutActors, true, true);

				for (auto ActorIter : OutActors)
				{
					auto SceneElementBasePtr = Cast<ASceneElement_DeviceBase>(ActorIter);
					if (SceneElementBasePtr)
					{
						if (SceneElementSet.Contains(SceneElementBasePtr))
						{
						}
						else
						{
							NeedDisplayAry.Add(SceneElementBasePtr);
						}
					}
				}
				continue;
			}

			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedDisplayAry.Add(SceneElementBasePtr);
				}
			}
		}
	}
	return false;
}

bool UGT_SwitchSceneElement_Device::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_Device::ProcessTask_SwitchState()
{
	if (SceneElementSet.IsEmpty())
	{
		return Super::ProcessTask_SwitchState();
	}
	else
	{
		FSceneElementConditional TempFilterTags;

		TempFilterTags.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Mode_View);
		// TempFilterTags.ConditionalSet.AddTag(FloorTag);

		for (auto Iter : SceneElementSet)
		{
			Iter->UpdateInteractionType(TempFilterTags);
		}
		SceneElementSet.Empty();
		return true;
	}
}

void UGT_SwitchSceneElement_BatchDevicesControl::Activate()
{
	Super::Activate();

	FilterTags.ConditionalSet = FGameplayTagContainer{FloorTag};
}

bool UGT_SwitchSceneElement_BatchDevicesControl::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	// for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	// {
	// 	if (FloorIter.Value->GameplayTagContainer.HasTag(FloorTag))
	// 	{
	// 	}
	// 	else
	// 	{
	// 		continue;
	// 	}
	//
	// 	NeedDisplayAry.Add(FloorIter.Value.LoadSynchronous());
	// 	for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
	// 	{
	// 		if (Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Soft))
	// 		{
	// 			TArray<AActor*> OutActors;
	//
	// 			Iter.Value->GetAttachedActors(OutActors, true, true);
	//
	// 			for (auto ActorIter : OutActors)
	// 			{
	// 				auto SceneElementBasePtr = Cast<ASceneElement_DeviceBase>(ActorIter);
	// 				if (SceneElementBasePtr)
	// 				{
	// 					if (SceneElementSet.Contains(SceneElementBasePtr))
	// 					{
	// 						PRINTFUNC();
	// 					}
	// 					else
	// 					{
	// 						NeedDisplayAry.Add(SceneElementBasePtr);
	// 					}
	// 				}
	// 			}
	// 			continue;
	// 		}
	//
	// 		TArray<AActor*> OutActors;
	//
	// 		Iter.Value->GetAttachedActors(OutActors, true, true);
	//
	// 		for (auto ActorIter : OutActors)
	// 		{
	// 			auto SceneElementBasePtr = Cast<ASceneElement_DeviceBase>(ActorIter);
	// 			if (SceneElementBasePtr)
	// 			{
	// 				if (SceneElementSet.Contains(SceneElementBasePtr))
	// 				{
	// 					PRINTFUNC();
	// 				}
	// 				else
	// 				{
	// 					NeedDisplayAry.Add(SceneElementBasePtr);
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	return false;
}

bool UGT_SwitchSceneElement_BatchDevicesControl::ProcessTask_Hiden()
{
	// for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	// {
	// 	if (FloorIter.Value->GameplayTagContainer.HasTag(FloorTag))
	// 	{
	// 		continue;
	// 	}
	// 	else
	// 	{
	// 	}
	//
	// 	NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
	// 	for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
	// 	{
	// 		TArray<AActor*> OutActors;
	//
	// 		Iter.Value->GetAttachedActors(OutActors, true, true);
	//
	// 		for (auto ActorIter : OutActors)
	// 		{
	// 			auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
	// 			if (SceneElementBasePtr)
	// 			{
	// 				NeedHideAry.Add(SceneElementBasePtr);
	// 			}
	// 		}
	// 	}
	// }
	//
	// for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	// {
	// 	NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
	// 	for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
	// 	{
	// 		TArray<AActor*> OutActors;
	//
	// 		Iter.Value->GetAttachedActors(OutActors, true, true);
	//
	// 		for (auto ActorIter : OutActors)
	// 		{
	// 			auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
	// 			if (SceneElementBasePtr)
	// 			{
	// 				NeedHideAry.Add(SceneElementBasePtr);
	// 			}
	// 		}
	// 	}
	// }

	return false;
}

bool UGT_SwitchSceneElement_BatchDevicesControl::ProcessTask_SwitchState()
{
	if (SceneElementSet.IsEmpty())
	{
		return Super::ProcessTask_SwitchState();
	}
	else
	{
		FSceneElementConditional TempFilterTags;

		TempFilterTags.ConditionalSet.AddTag(USmartCitySuiteTags::Interaction_Mode_BatchControl);
		// TempFilterTags.ConditionalSet.AddTag(FloorTag);

		for (auto Iter : SceneElementSet)
		{
			Iter->UpdateInteractionType(TempFilterTags);
		}
		SceneElementSet.Empty();
		return true;
	}
}

bool UGT_SwitchSceneElement_SpecialArea::ProcessTask_Display()
{
	Super::ProcessTask_Display();

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
		}
		else
		{
			continue;
		}

		NeedDisplayAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedDisplayAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_SpecialArea::ProcessTask_Hiden()
{
	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (PriorityHideFloorSet.Contains(FloorIter.Value->FloorTag))
		{
		}
		else
		{
			continue;
		}

		PriorityHideFloorAryAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					PriorityHideFloorAryAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (FloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		if (PriorityHideFloorSet.Contains(FloorIter.Value->FloorTag))
		{
			continue;
		}
		else
		{
		}

		NeedHideAry.Add(FloorIter.Value.LoadSynchronous());
		for (const auto& Iter : FloorIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	for (const auto& LandScapeIter : UAssetRefMap::GetInstance()->LandScapeHelper)
	{
		NeedHideAry.Add(LandScapeIter.Value.LoadSynchronous());
		for (const auto& Iter : LandScapeIter.Value.LoadSynchronous()->SceneElementCategoryMap)
		{
			TArray<AActor*> OutActors;

			Iter.Value->GetAttachedActors(OutActors, true, true);

			for (auto ActorIter : OutActors)
			{
				auto SceneElementBasePtr = Cast<ASceneElementBase>(ActorIter);
				if (SceneElementBasePtr)
				{
					NeedHideAry.Add(SceneElementBasePtr);
				}
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElement_SpecialArea::ProcessTask_SwitchState()
{
	if (PriorityHideFloorAryIndex < PriorityHideFloorAryAry.Num())
	{
		ON_SCOPE_EXIT
		{
			PriorityHideFloorAryIndex++;
		};

		auto ActorPtr = PriorityHideFloorAryAry[PriorityHideFloorAryIndex];
		if (ActorPtr)
		{
			auto SceneElementPtr = Cast<ASceneElementBase>(ActorPtr);
			if (SceneElementPtr)
			{
				SceneElementPtr->UpdateInteractionType(FSceneElementConditional::EmptyConditional);
			}
			else
			{
				ActorPtr->SetActorHiddenInGame(true);

				TArray<AActor*> OutActors;

				ActorPtr->GetAttachedActors(OutActors, true, true);
				for (auto Iter : OutActors)
				{
					Iter->SetActorHiddenInGame(true);
				}
			}
		}

		return true;
	}

	return Super::ProcessTask_SwitchState();
}
