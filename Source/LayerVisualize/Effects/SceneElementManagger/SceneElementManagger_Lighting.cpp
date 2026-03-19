#include "SceneElementManagger_Lighting.h"

#include "ActorSequenceComponent.h"
#include "AssetRefMap.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetStringLibrary.h"

#include "CollisionDataStruct.h"
#include "FloorHelper.h"
#include "GameplayTagsLibrary.h"
#include "PlanetPlayerController.h"
#include "SmartCitySuiteTags.h"
#include "Components/InstancedStaticMeshComponent.h"

bool UGT_SwitchSceneElementManagger_Lighting_Display::ProcessTask(
	float DeltaTime
	)
{
	Super::ProcessTask(DeltaTime);

	if (DisplayIndex < DisplayAry.Num())
	{
		auto& ModelIndex = DisplayAry[DisplayIndex];
		if (SceneElementManaggerPtr->ISMAry.IsValidIndex(ModelIndex.ModelAryIndex))
		{
			auto ISMPtr = SceneElementManaggerPtr->ISMAry[ModelIndex.ModelAryIndex];
			if (ModelIndex.ModelInsID.IsValid() && ISMPtr->IsValidId(
				     ModelIndex.ModelInsID
				    ))
			{
				return true;
			}
			else
			{
				ModelIndex.ModelInsID = ISMPtr->
					AddInstanceById(ModelIndex.Transform);

				return true;
			}
		}
	}

	return false;
}

bool UGT_SwitchSceneElementManagger_Lighting_Hide::ProcessTask(
	float DeltaTime
	)
{
	Super::ProcessTask(DeltaTime);

	if (HideIndex < HideAry.Num())
	{
		auto& ModelIndex = HideAry[HideIndex];
		if (SceneElementManaggerPtr->ISMAry.IsValidIndex(ModelIndex.ModelAryIndex))
		{
			auto ISMPtr = SceneElementManaggerPtr->ISMAry[ModelIndex.ModelAryIndex];
			if (ISMPtr->IsValidId(ModelIndex.ModelInsID))
			{
				ISMPtr->
					RemoveInstanceById(ModelIndex.ModelInsID);

				ModelIndex.ModelInsID = FPrimitiveInstanceId();

				return true;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

ASceneElementManagger_Lighting::ASceneElementManagger_Lighting(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
}

void ASceneElementManagger_Lighting::Merge(
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
			ON_SCOPE_EXIT
			{
				ActorRef->Destroy();
			};

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

			const auto Str = (InUserData.Key.ToString().Append(InUserData.Value));

			FModelIndex ModelIndex;

			for (int32 Index = 0; Index < TempIndex.Num(); Index++)
			{
				if (TempIndex[Index] == Str)
				{
					const auto Transform =
						STPtr->GetStaticMeshComponent()->
						       GetComponentTransform();

					ModelIndex.ModelAryIndex = ISMAry.Num();
					ModelIndex.ModelInsID = ISMAry[Index]->AddInstanceById(Transform);
					ModelIndex.Transform = Transform;

					FloorIndexMap.Add(FloorPtr->FloorIndex, {ModelIndex});

					return;
				}
			}


			auto NewSTMComponentPtr = Cast<UInstancedStaticMeshComponent>(
			                                                              AddComponentByClass(
				                                                               UInstancedStaticMeshComponent::StaticClass(),
				                                                               true,
				                                                               FTransform::Identity,
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

			auto Transform =
				STPtr->GetStaticMeshComponent()->
				       GetComponentTransform();

			TempIndex.Add(Str);
			ISMAry.Add(NewSTMComponentPtr);

			ModelIndex.ModelAryIndex = ISMAry.Num();
			ModelIndex.ModelInsID = NewSTMComponentPtr->AddInstanceById(Transform);
			ModelIndex.Transform = Transform;

			FloorIndexMap.Add(FloorPtr->FloorIndex, {ModelIndex});
		}
	}
}

TArray<TObjectPtr<UGameplayTaskBase>> ASceneElementManagger_Lighting::SwitchInteractionType(
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
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Floor)
		)
		{
			SetActorHiddenInGame(false);


			Result.Append(SwitchFloor(ConditionalSet));
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

void ASceneElementManagger_Lighting::UpdateExtensionParamMap(
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
	}
}

TArray<TObjectPtr<UGameplayTaskBase>> ASceneElementManagger_Lighting::SwitchFloor(
	const FSceneElementConditional& ConditionalSet
	) const
{
	TArray<TObjectPtr<UGameplayTaskBase>> Result;

	auto PCPtr = Cast<APlanetPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorldImp()));

	int32 FloorIndex = 0;
	for (auto Iter : UAssetRefMap::GetInstance()->FloorHelpers)
	{
		if (ConditionalSet.ConditionalSet.HasTag(Iter.Key))
		{
			FloorIndex = Iter.Value->FloorIndex;
		}
	}

	{
		auto GameplayTaskPtr = UGameplayTask::NewTask<UGT_SwitchSceneElementManagger_Lighting_Display>(
			 TScriptInterface<
				 IGameplayTaskOwnerInterface>(
				                              PCPtr
				                             )
			);

		for (const auto& Iter : FloorIndexMap)
		{
			if (Iter.Key == FloorIndex)
			{
				GameplayTaskPtr->DisplayAry.Append(Iter.Value);
			}
		}

		Result.Add(GameplayTaskPtr);
	}
	{
		auto GameplayTaskPtr = UGameplayTask::NewTask<UGT_SwitchSceneElementManagger_Lighting_Hide>(
			 TScriptInterface<
				 IGameplayTaskOwnerInterface>(
				                              PCPtr
				                             )
			);

		for (const auto& Iter : FloorIndexMap)
		{
			if (Iter.Key != FloorIndex)
			{
				GameplayTaskPtr->HideAry.Append(Iter.Value);
			}
		}

		Result.Add(GameplayTaskPtr);
	}

	return Result;
}
