
#include "SceneInteractionDecorator_Addtional.h"

#include "AssetRefMap.h"
#include "FloorHelper.h"
#include "MessageBody.h"
#include "SceneElementCategory.h"
#include "SceneElementManagger_AccessControl.h"
#include "SceneElement_Space.h"
#include "SceneInteractionDecorator_Area.h"
#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

FAddtional_Decorator::FAddtional_Decorator() :
                                           Super(
                                                )
{
}

void FAddtional_Decorator::Entry()
{
	FDecoratorBase::Entry();

	Initial();
}

void FAddtional_Decorator::ReEntry()
{
	FDecoratorBase::ReEntry();

	Initial();
}

void FAddtional_Decorator::Quit()
{
	bIsEnd = true;
	
	FDecoratorBase::Quit();
}

bool FAddtional_Decorator::IsEnd() const
{
	return bIsEnd;
}

void FAddtional_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	FDecoratorBase::OnOtherDecoratorEntry(NewDecoratorSPtr);

	Process();
}

void FAddtional_Decorator::Initial()
{
	SceneElementSet.Empty();

	auto AreaDecoratorSPtr =
		DynamicCastSharedPtr<FArea_Decorator>(
		                                      USceneInteractionWorldSystem::GetInstance()->GetDecorator(
			                                       USmartCitySuiteTags::Interaction_Area
			                                      )
		                                     );

	if (!AreaDecoratorSPtr)
	{
		return;
	}


	if (
		AreaDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor)
	)
	{
		auto ViewFloor_DecoratorSPtr = DynamicCastSharedPtr<FFloor_Decorator>(AreaDecoratorSPtr);

		FloorTag = AreaDecoratorSPtr->GetBranchDecoratorType();

		for (const auto& FloorIter : UAssetRefMap::GetInstance()->FloorHelpers)
		{
		if (FloorTag.MatchesTag( FloorIter.Value->FloorTag))
			{
				auto MessageSPtr = MakeShared<FMessageBody_SelectedFloor>();

				for (auto Iter : FloorIter.Value->SceneElementCategoryMap)
				{
					if (
						Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Soft) ||
						Iter.Key.MatchesTag(USmartCitySuiteTags::SceneElement_Category_Space)
					)
					{
					}
					else
					{
						continue;
					}

					TArray<AActor*> OutActors;
					Iter.Value->GetAttachedActors(OutActors, true, true);

					for (auto SpaceIter : OutActors)
					{
						auto SpacePtr = Cast<ASceneElement_Space>(SpaceIter);
						if (SpacePtr)
						{
							const auto DevicesSet = SpacePtr->GetAllDevices();

							for (const auto& DeviceIter : DevicesSet)
							{
								if (DeviceIter)
								{
									if (ExtensionParamMap.Contains(DeviceIter->DeviceTypeStr))
									{
										DeviceIter->UpdateExtensionParamMap(
										                                    ExtensionParamMap[DeviceIter->
											                                    DeviceTypeStr],
										                                    false
										                                   );
									}
									else
									{
										DeviceIter->UpdateExtensionParamMap(
										                                    {},
										                                    false
										                                   );
									}
								}
							}
						}
					}
				}

				return;
			}
		}
	}

	if (
		AreaDecoratorSPtr->GetBranchDecoratorType().MatchesTag(USmartCitySuiteTags::Interaction_Area_Space)
	)
	{
		auto ViewSpace_DecoratorSPtr = DynamicCastSharedPtr<FViewSpace_Decorator>(AreaDecoratorSPtr);

		FloorTag = ViewSpace_DecoratorSPtr->SceneElementPtr->BelongFloor->FloorTag;

		const auto DevicesSet = ViewSpace_DecoratorSPtr->SceneElementPtr->GetAllDevices();

		SceneElementSet.Append(DevicesSet);

		for (const auto& DeviceIter : DevicesSet)
		{
			if (DeviceIter)
			{
				if (ExtensionParamMap.Contains(DeviceIter->DeviceTypeStr))
				{
					DeviceIter->UpdateExtensionParamMap(
					                                    ExtensionParamMap[DeviceIter->DeviceTypeStr],
					                                    false
					                                   );
				}
				else
				{
					DeviceIter->UpdateExtensionParamMap(
					                                    {},
					                                    false
					                                   );
				}
			}
		}

		return;
	}
}

void FAddtional_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	FDecoratorBase::OnUpdateFilterComplete(bIsOK, TaskPtr);

	Process();
}

void FAddtional_Decorator::Process()
{
	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddLambda([this](auto,auto)
	{

		bIsEnd = true;
	});

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_BatchControlDevice(
			 SceneActorConditional,
			 true,
			 false,
			 MulticastDelegate,
			 SceneElementSet,
			 FloorTag
			);
	};
}
