#include "SceneElementManagger_AccessControl.h"

#include "ActorSequenceComponent.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetStringLibrary.h"

#include "CollisionDataStruct.h"
#include "FloorHelper.h"
#include "SmartCitySuiteTags.h"

ASceneElementManagger_AccessControl::ASceneElementManagger_AccessControl(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
}

void ASceneElementManagger_AccessControl::Merge(
	const TSoftObjectPtr<AActor>& ActorRef,
	const TPair<FName, FString>& InUserData,
	const TMap<FName, FString>& NewUserData
	)
{
	Super::Merge(ActorRef, InUserData, NewUserData);

	if (ActorRef.ToSoftObjectPath().IsValid())
	{
		auto STPtr = Cast<AStaticMeshActor>(ActorRef.Get());
		if (STPtr)
		{
			auto ParentPtr = ActorRef->GetAttachParentActor();
			AFloorHelper* FloorPtr = nullptr;
			for (; ParentPtr;)
			{
				ParentPtr = ParentPtr->GetAttachParentActor();
				FloorPtr = Cast<AFloorHelper>(ParentPtr);
				if (FloorPtr)
				{
					break;
				}
			}

			if (FloorPtr)
			{
			}
			else
			{
				checkNoEntry();
			}

			if (
				NewUserData.Contains(TEXT("Element*Family")) &&
				(NewUserData[TEXT("Element*Family")] == TEXT("翼式闸机"))
			)
			{
				for (const auto& Iter : STPtr->Tags)
				{
					auto Transform =
						STPtr->GetStaticMeshComponent()->
						       GetComponentTransform();

					auto NewSTMComponentPtr = Cast<UStaticMeshComponent>(
					                                                     AddComponentByClass(
						                                                      UStaticMeshComponent::StaticClass(),
						                                                      true,
						                                                      Transform,
						                                                      false
						                                                     )
					                                                    );

					NewSTMComponentPtr->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());
					for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
					{
						NewSTMComponentPtr->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
					}

					NewSTMComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

					NewSTMComponentPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					NewSTMComponentPtr->SetCollisionObjectType(Device_Object);
					NewSTMComponentPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

					NewSTMComponentPtr->SetCollisionResponseToChannel(
					                                                  ExternalWall_Object,
					                                                  ECollisionResponse::ECR_Overlap
					                                                 );
					NewSTMComponentPtr->SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
					NewSTMComponentPtr->SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);

					NewSTMComponentPtr->SetRenderCustomDepth(false);

					FRegexPattern Pattern(TEXT(R"(Group:(\d+))"));
					FRegexMatcher Matcher(Pattern, Iter.ToString());

					if (Matcher.FindNext())
					{
						FSceneElementManaggerKeyHelper SceneElementManaggerKeyHelper;

						if (FloorPtr)
						{
							SceneElementManaggerKeyHelper.FloorTag = FloorPtr->FloorTag;
						}

						FString Index = Matcher.GetCaptureGroup(1);

						SceneElementManaggerKeyHelper.Group = UKismetStringLibrary::Conv_StringToInt(Index);

						ChestMeshComponent.Add(NewSTMComponentPtr);

						break;
					}
				}
			}
			else if (
				NewUserData.Contains(TEXT("Element*Type")) &&
				(NewUserData[TEXT("Element*Type")] == TEXT("闸机门"))
			)
			{
				FSceneElementManaggerKeyHelper SceneElementManaggerKeyHelper;

				bool bHasGroup = false;
				bool bHasDoorInfo = false;
				for (const auto& Iter : STPtr->Tags)
				{
					{
						FRegexPattern Pattern(TEXT(R"(Group:(\d+))"));
						FRegexMatcher GroupMatcher(Pattern, Iter.ToString());
						if (GroupMatcher.FindNext())
						{
							FString GroupIndex = GroupMatcher.GetCaptureGroup(1);

							SceneElementManaggerKeyHelper.Group = UKismetStringLibrary::Conv_StringToInt(GroupIndex);

							bHasGroup = true;

							if (bHasGroup && bHasDoorInfo)
							{
								break;
							}
						}
					}
					{
						FRegexPattern Pattern(TEXT(R"(Direction:(\d+))"));
						FRegexMatcher GroupMatcher(Pattern, Iter.ToString());
						if (GroupMatcher.FindNext())
						{
							bHasDoorInfo = true;

							if (bHasGroup && bHasDoorInfo)
							{
								break;
							}
						}
					}
				}

				if (bHasGroup && bHasDoorInfo)
				{
					if (FloorPtr)
					{
						SceneElementManaggerKeyHelper.FloorTag = FloorPtr->FloorTag;
					}

					auto Transform =
						STPtr->GetStaticMeshComponent()->
						       GetComponentTransform();

					auto SocketTransform =
						STPtr->GetStaticMeshComponent()->
						       GetSocketTransform(TEXT("Socket"));

					auto NewSceneComponentPtr = Cast<USceneComponent>(
					                                                  AddComponentByClass(
						                                                   USceneComponent::StaticClass(),
						                                                   true,
						                                                   SocketTransform,
						                                                   false
						                                                  )
					                                                 );

					NewSceneComponentPtr->AttachToComponent(
					                                        RootComponent,
					                                        FAttachmentTransformRules::KeepWorldTransform
					                                       );

					DrawDebugSphere(
					                GetWorld(),
					                NewSceneComponentPtr->GetComponentLocation(),
					                20,
					                20,
					                FColor::Red,
					                true
					               );

					if (FanAnchorhComponent.Contains(SceneElementManaggerKeyHelper))
					{
						FanAnchorhComponent[SceneElementManaggerKeyHelper].Add({NewSceneComponentPtr, bHasDoorInfo});
					}
					else
					{
						FanAnchorhComponent.Add(SceneElementManaggerKeyHelper, {{NewSceneComponentPtr, bHasDoorInfo}});
					}

					auto NewSTMComponentPtr = Cast<UStaticMeshComponent>(
					                                                     AddComponentByClass(
						                                                      UStaticMeshComponent::StaticClass(),
						                                                      true,
						                                                      Transform,
						                                                      false
						                                                     )
					                                                    );

					NewSTMComponentPtr->AttachToComponent(
					                                      NewSceneComponentPtr,
					                                      FAttachmentTransformRules::KeepWorldTransform
					                                     );

					NewSTMComponentPtr->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());
					for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
					{
						NewSTMComponentPtr->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
					}

					NewSTMComponentPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					NewSTMComponentPtr->SetCollisionObjectType(Device_Object);
					NewSTMComponentPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

					NewSTMComponentPtr->SetCollisionResponseToChannel(
					                                                  ExternalWall_Object,
					                                                  ECollisionResponse::ECR_Overlap
					                                                 );
					NewSTMComponentPtr->
						SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
					NewSTMComponentPtr->
						SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);

					NewSTMComponentPtr->SetRenderCustomDepth(false);

					FanMeshComponent.Add(NewSTMComponentPtr);
				}
			}
		}

		ActorRef->Destroy();
	}
}

TArray<TObjectPtr<UGameplayTaskBase>> ASceneElementManagger_AccessControl::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	auto Result = Super::SwitchInteractionType(ConditionalSet);


	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) &&
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_ViewElevator)
		)
		{
			SetActorHiddenInGame(true);

			return Result;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) ||
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			SetActorHiddenInGame(true);

			return Result;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Floor_F1)
		)
		{
			SetActorHiddenInGame(false);

			return Result;
		}
	}
	{
		auto EmptyContainer = FGameplayTagContainer::EmptyContainer;

		EmptyContainer.AddTag(USmartCitySuiteTags::Interaction_Area_Floor.GetTag());

		if (ConditionalSet.ConditionalSet.HasAll(EmptyContainer) && ConditionalSet.ConditionalSet.Num() ==
		    EmptyContainer.Num())
		{
			SetActorHiddenInGame(true);

			return Result;
		}
	}
	{
		auto EmptyContainer = FGameplayTagContainer::EmptyContainer;

		EmptyContainer.AddTag(USmartCitySuiteTags::Interaction_Area_Floor);
		EmptyContainer.AddTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger);

		if (ConditionalSet.ConditionalSet.HasAll(EmptyContainer) && ConditionalSet.ConditionalSet.Num() ==
		    EmptyContainer.Num())
		{
			SetActorHiddenInGame(true);

			return Result;
		}
	}
	{
		if (ConditionalSet.ConditionalSet.IsEmpty())
		{
		}
		SetActorHiddenInGame(true);

		return Result;
	}
}

void ASceneElementManagger_AccessControl::UpdateExtensionParamMap(
	const TMap<FString, FString>& NewExtensionParamMap,
	bool bImmediatelyUpdate
	)
{
	Super::UpdateExtensionParamMap(NewExtensionParamMap, bImmediatelyUpdate);

	if (
		NewExtensionParamMap.Contains(TEXT("Floor")) &&
		NewExtensionParamMap.Contains(TEXT("Group")) &&
		NewExtensionParamMap.Contains(TEXT("State"))
	)
	{
		FSceneElementManaggerKeyHelper SceneElementManaggerKeyHelper;

		SceneElementManaggerKeyHelper.FloorTag = FGameplayTag::RequestGameplayTag(*NewExtensionParamMap[TEXT("Floor")]);
		SceneElementManaggerKeyHelper.Group = UKismetStringLibrary::Conv_StringToInt(
			 NewExtensionParamMap[TEXT("Group")]
			);

		TargetRotatorsMap.Add(
		                      SceneElementManaggerKeyHelper,
		                      UKismetStringLibrary::Conv_StringToInt(NewExtensionParamMap[TEXT("State")]) ? 90 : 0
		                     );

		GetWorldTimerManager().ClearTimer(RotTimersMap);
		GetWorldTimerManager().SetTimer(RotTimersMap, this, &ThisClass::RotationImp, 1.f / 24, true);
	}
}

void ASceneElementManagger_AccessControl::RotationImp()
{
	for (auto Iter = TargetRotatorsMap.CreateIterator(); Iter; ++Iter)
	{
		bool bIsOK = true;
		if (FanAnchorhComponent.Contains(Iter->Key))
		{
			FRotator NewRot = FRotator::ZeroRotator;
			// if (Iter->Value > 45.f)
			// {
			// 	for (auto SecondIter : FanAnchorhComponent[Iter->Key])
			// 	{
			// 		NewRot = SecondIter->GetRelativeRotation() +
			// 		         FRotator(0.f, RotSpeed * 1.f / 24, 0.f);
			//
			// 		NewRot.Yaw = FMath::Clamp(NewRot.Yaw, 0.f, 90.f);
			//
			// 		SecondIter->SetRelativeRotation(NewRot);
			// 	}
			//
			// 	if (!FMath::IsNearlyEqual(NewRot.Yaw, Iter->Value))
			// 	{
			// 		bIsOK = false;
			// 	}
			// }
			// else
			// {
			// 	for (auto SecondIter : FanAnchorhComponent[Iter->Key])
			// 	{
			// 		NewRot = SecondIter->GetRelativeRotation() -
			// 		         FRotator(0.f, RotSpeed * 1.f / 24, 0.f);
			//
			// 		NewRot.Yaw = FMath::Clamp(NewRot.Yaw, 0.f, 90.f);
			//
			// 		SecondIter->SetRelativeRotation(NewRot);
			// 	}
			//
			// 	if (!FMath::IsNearlyEqual(NewRot.Yaw, Iter->Value))
			// 	{
			// 		bIsOK = false;
			// 	}
			// }
		}

		if (bIsOK)
		{
			Iter.RemoveCurrent();
		}
	}

	if (TargetRotatorsMap.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(RotTimersMap);
	}
}
