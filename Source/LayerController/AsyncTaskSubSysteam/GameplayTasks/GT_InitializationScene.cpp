#include "GT_InitializationScene.h"

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

UGT_InitializeSceneActors::UGT_InitializeSceneActors(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsPausable = true;

	Priority = FGameplayTasks::DefaultPriority;

	UseScopeType = EUseScopeType::kCount;

	PerTickProcessNum = 1000;
}

void UGT_InitializeSceneActors::Activate()
{
	Super::Activate();
}

void UGT_InitializeSceneActors::TickTask(
	float DeltaTime
	)
{
	Super::TickTask(DeltaTime);
}

bool UGT_InitializeSceneActors::ProcessTask(
	float DeltaTime
	)
{
	Super::ProcessTask(DeltaTime);

	switch (Step)
	{
	case EStep::kFloor_ControlBorder:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
			{
				ProcessTask_ControlBorder(Iter.Value);
			}

			Step = EStep::kFloor_AS;
			return true;
		}
		break;
	case EStep::kFloor_AS:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
			{
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_AS;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_StructItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
			}


			Step = EStep::kFloor_ID;
			return true;
		}
		break;
	case EStep::kFloor_ID:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
			{
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_ID;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_InnerStructItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
			}

			Step = EStep::kFloor_Space;
			return true;
		}
		break;
	case EStep::kFloor_Space:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
			{
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_Space;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_SpaceItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
			}


			Step = EStep::kFloor_Sost;
			return true;
		}
		break;
	case EStep::kFloor_Sost:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->FloorHelpers)
			{
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_Soft;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_SoftDecorationItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
			}


			Step = EStep::kBuilding;
			return true;
		}
		break;
	case EStep::kBuilding:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->BuildingHelpers)
			{
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_AS;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_StructItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_ID;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_InnerStructItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_Soft;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_SoftDecorationItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
				{
					auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
					SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_Space;

					Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
					                                                          SceneElementCategoryPtr->Category,
					                                                          SceneElementCategoryPtr
					                                                         );

					SceneElementCategoryPtr->AttachToActor(
					                                       Iter.Value.LoadSynchronous(),
					                                       FAttachmentTransformRules::KeepWorldTransform
					                                      );

					ProcessTask_SpaceItemSet(SceneElementCategoryPtr, Iter.Value->AllReference);
				}
			}


			Step = EStep::kLandscape;
			return true;
		}
		break;
	case EStep::kLandscape:
		{
			for (const auto& Iter : UAssetRefMap::GetInstance()->LandScapeHelper)
			{
				auto SceneElementCategoryPtr = GetWorld()->SpawnActor<ASceneElementCategory>();
				SceneElementCategoryPtr->Category = USmartCitySuiteTags::SceneElement_Category_Space;

				Iter.Value.LoadSynchronous()->SceneElementCategoryMap.Add(
				                                                          SceneElementCategoryPtr->Category,
				                                                          SceneElementCategoryPtr
				                                                         );

				SceneElementCategoryPtr->AttachToActor(
				                                       Iter.Value.LoadSynchronous(),
				                                       FAttachmentTransformRules::KeepWorldTransform
				                                      );

				ProcessTask_Lanscape(
				                     SceneElementCategoryPtr,
				                     Iter.Value->AllReference.StructItemSet.DatasmithSceneActorSet
				                    );
				ProcessTask_Lanscape(
				                     SceneElementCategoryPtr,
				                     Iter.Value->AllReference.InnerStructItemSet.DatasmithSceneActorSet
				                    );
				ProcessTask_Lanscape(
				                     SceneElementCategoryPtr,
				                     Iter.Value->AllReference.SoftDecorationItem.DatasmithSceneActorSet
				                    );
				ProcessTask_Lanscape(
				                     SceneElementCategoryPtr,
				                     Iter.Value->AllReference.SpaceItemSet.DatasmithSceneActorSet
				                    );
			}


			Step = EStep::kComplete;
			return true;
		}
		break;
	case EStep::kComplete:
	default:
		{
			return false;
		}
	}
}

void UGT_InitializeSceneActors::OnDestroy(
	bool bInOwnerFinished
	)
{
	OnEnd.Broadcast(true);

	Super::OnDestroy(bInOwnerFinished);
}

void UGT_InitializeSceneActors::ProcessTask_ControlBorder(
	const TSoftObjectPtr<AFloorHelper>& FloorHelperRef
	)
{
	ASceneElement_ControlBorder* SceneElement_ControlBorderPtr = nullptr;
	for (const auto& ControlBorderIter : FloorHelperRef.LoadSynchronous()->ControlBorderMaps)
	{
		{
			auto Components = ControlBorderIter.LoadSynchronous()->P2->GetComponents();
			for (auto SecondIter : Components)
			{
				auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
				if (!InterfacePtr)
				{
					continue;
				}
				auto AUDPtr = Cast<UDatasmithAssetUserData>(
				                                            InterfacePtr->GetAssetUserDataOfClass(
					                                             UDatasmithAssetUserData::StaticClass()
					                                            )
				                                           );
				if (!AUDPtr)
				{
					continue;
				}

				auto Datasmith_UniqueId = AUDPtr->MetaData.Find(TEXT("Datasmith_UniqueId"));
				if (!Datasmith_UniqueId)
				{
					continue;
				}

				SceneElement_ControlBorderPtr = GetWorld()->SpawnActor<ASceneElement_ControlBorder>();

				SceneElement_ControlBorderPtr->Replace(ControlBorderIter.LoadSynchronous()->P2, {}, AUDPtr->MetaData);;
				SceneElement_ControlBorderPtr->SceneElementID = *Datasmith_UniqueId;
				SceneElement_ControlBorderPtr->InitialSceneElement();

				USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
				                                             Add(
				                                                 SceneElement_ControlBorderPtr->SceneElementID,
				                                                 SceneElement_ControlBorderPtr
				                                                );
				break;
			}
		}
		{
			TMap<int32, ASceneElementBase*> MergeActorsMap;

			for (auto DSIter : ControlBorderIter.LoadSynchronous()->Devices)
			{
				bool bIsSceneElement = false;
				auto Components = DSIter->GetComponents();
				for (auto SecondIter : Components)
				{
					auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
					if (!InterfacePtr)
					{
						continue;
					}
					auto AUDPtr = Cast<UDatasmithAssetUserData>(
					                                            InterfacePtr->GetAssetUserDataOfClass(
						                                             UDatasmithAssetUserData::StaticClass()
						                                            )
					                                           );
					if (!AUDPtr)
					{
						continue;
					}

					auto Datasmith_UniqueId = AUDPtr->MetaData.Find(TEXT("Datasmith_UniqueId"));
					if (!Datasmith_UniqueId)
					{
						continue;
					}

					for (const auto& ThirdIter : UAssetRefMap::GetInstance()->NeedReplaceByUserData)
					{
						auto MetaDataIter = AUDPtr->MetaData.Find(*ThirdIter.Key.Key);
						if (!MetaDataIter)
						{
							continue;
						}
						if (ThirdIter.Key.bOnlyKey)
						{
						}
						else if (ThirdIter.Key.bSkip)
						{
							bIsSceneElement = true;
							continue;
						}
						else
						{
							if (*MetaDataIter != ThirdIter.Key.Value)
							{
								continue;
							}
						}

						{
							auto NewActorPtr = GetWorld()->SpawnActor<ASceneElementBase>(
								 ThirdIter.Value
								);
							NewActorPtr->Replace(DSIter, {*ThirdIter.Key.Key, *MetaDataIter}, AUDPtr->MetaData);

							// NewActorPtr->AttachToActor(SceneElement_ControlBorderPtr, FAttachmentTransformRules::KeepWorldTransform);

							NewActorPtr->SceneElementID = *Datasmith_UniqueId;
							NewActorPtr->InitialSceneElement();
							NewActorPtr->QuitAllState();

							USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
							                                             Add(
							                                                 NewActorPtr->SceneElementID,
							                                                 NewActorPtr
							                                                );

							SceneElement_ControlBorderPtr->ControlDevicesSet.Add(NewActorPtr);
						}
						bIsSceneElement = true;
						break;
					}

					for (const auto& ThirdIter : UAssetRefMap::GetInstance()->NeedMergeByUserData)
					{
						auto MetaDataIter = AUDPtr->MetaData.Find(*ThirdIter.Key.Key);
						if (!MetaDataIter)
						{
							continue;
						}

						if (ThirdIter.Key.bOnlyKey)
						{
						}
						else
						{
							if (*MetaDataIter != ThirdIter.Key.Value)
							{
								continue;
							}
						}

						auto HashCode = HashCombine(
						                            GetTypeHash(*MetaDataIter),
						                            GetTypeHash(ThirdIter.Key)
						                           );

						if (MergeActorsMap.Contains(HashCode))
						{
							MergeActorsMap[HashCode]->Merge(
							                                DSIter,
							                                {*ThirdIter.Key.Key, *MetaDataIter},
							                                AUDPtr->MetaData
							                               );
						}
						else
						{
							auto NewActorPtr = GetWorld()->SpawnActor<ASceneElementBase>(
								 ThirdIter.Value
								);
							NewActorPtr->Merge(DSIter, {*ThirdIter.Key.Key, *MetaDataIter}, AUDPtr->MetaData);

							// NewActorPtr->AttachToActor(SceneElement_ControlBorderPtr, FAttachmentTransformRules::KeepWorldTransform);

							NewActorPtr->SceneElementID = *Datasmith_UniqueId;
							NewActorPtr->InitialSceneElement();
							NewActorPtr->QuitAllState();


							MergeActorsMap.Add(HashCode, NewActorPtr);
							USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
							                                             Add(
							                                                 NewActorPtr->SceneElementID,
							                                                 NewActorPtr
							                                                );

							SceneElement_ControlBorderPtr->ControlDevicesSet.Add(NewActorPtr);
						}

						bIsSceneElement = true;
						break;
					}

					if (bIsSceneElement)
					{
						break;
					}
				}

				if (bIsSceneElement)
				{
				}
				else
				{
					if (DSIter->IsA(ASceneElementBase::StaticClass()))
					{
						continue;
					}

					if (DSIter->IsA(AStaticMeshActor::StaticClass()))
					{
						auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_Regualar>(
							);
						NewActorPtr->Replace(DSIter, {}, {});

						// NewActorPtr->AttachToActor(SceneElement_ControlBorderPtr, FAttachmentTransformRules::KeepWorldTransform);

						NewActorPtr->InitialSceneElement();
						NewActorPtr->QuitAllState();

						SceneElement_ControlBorderPtr->ControlDevicesSet.Add(NewActorPtr);
					}
					else
					{
						auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_Regualar>(
							);
						NewActorPtr->Replace(DSIter, {}, {});

						// NewActorPtr->AttachToActor(SceneElement_ControlBorderPtr, FAttachmentTransformRules::KeepWorldTransform);

						NewActorPtr->InitialSceneElement();
						NewActorPtr->QuitAllState();

						SceneElement_ControlBorderPtr->ControlDevicesSet.Add(NewActorPtr);
					}
				}
			}
		}
		{
			for (auto DSIter : ControlBorderIter.LoadSynchronous()->CurtainWall)
			{
				auto Components = DSIter->GetComponents();
				for (auto SecondIter : Components)
				{
					auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
					if (!InterfacePtr)
					{
						continue;
					}
					auto AUDPtr = Cast<UDatasmithAssetUserData>(
					                                            InterfacePtr->GetAssetUserDataOfClass(
						                                             UDatasmithAssetUserData::StaticClass()
						                                            )
					                                           );
					if (!AUDPtr)
					{
						continue;
					}

					auto Datasmith_UniqueId = AUDPtr->MetaData.Find(TEXT("Datasmith_UniqueId"));
					if (!Datasmith_UniqueId)
					{
						continue;
					}

					{
						auto NewActorPtr = GetWorld()->SpawnActor<ABuilding_CurtainWall>();

						NewActorPtr->GenerateRollerBlind(DSIter.LoadSynchronous());

						NewActorPtr->Replace(DSIter, {}, AUDPtr->MetaData);

						NewActorPtr->SceneElementID = *Datasmith_UniqueId;
						NewActorPtr->InitialSceneElement();

						NewActorPtr->QuitAllState();

						USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
						                                             Add(
						                                                 NewActorPtr->SceneElementID,
						                                                 NewActorPtr
						                                                );

						SceneElement_ControlBorderPtr->ControlDevicesSet.Add(NewActorPtr);
					}

					break;
				}
			}
		}
	}
}

bool UGT_InitializeSceneActors::ProcessTask_StructItemSet(
	ASceneElementCategory* SceneElementCategoryPtr,
	FSceneElementMap& AllReference
	)
{
	return ProcessTask_Generic(SceneElementCategoryPtr, AllReference, AllReference.StructItemSet);
}

bool UGT_InitializeSceneActors::ProcessTask_InnerStructItemSet(
	ASceneElementCategory* SceneElementCategoryPtr,
	FSceneElementMap& AllReference
	)
{
	return ProcessTask_Generic(SceneElementCategoryPtr, AllReference, AllReference.InnerStructItemSet);
}

bool UGT_InitializeSceneActors::ProcessTask_SoftDecorationItemSet(
	ASceneElementCategory* SceneElementCategoryPtr,
	FSceneElementMap& AllReference
	)
{
	return ProcessTask_Generic(SceneElementCategoryPtr, AllReference, AllReference.SoftDecorationItem);
}

bool UGT_InitializeSceneActors::ProcessTask_SpaceItemSet(
	ASceneElementCategory* SceneElementCategoryPtr,
	FSceneElementMap& AllReference
	)
{
	const auto SpaceInfo = UAssetRefMap::GetInstance()->SpaceInfo;

	for (const auto& DSIter : AllReference.SpaceItemSet.DatasmithSceneActorSet)
	{
		DSIter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);

		TArray<AActor*> OutActors;

		DSIter.LoadSynchronous()->GetAttachedActors(OutActors, true, true);

		{
			TArray<AActor*> TempOutActors;

			DSIter.LoadSynchronous()->GetAttachedActors(TempOutActors, true);
			for (auto Iter : TempOutActors)
			{
				Iter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);
			}
		}

		TMap<int32, ASceneElementBase*> MergeActorsMap;

		for (auto Iter : OutActors)
		{
			if (Iter)
			{
				auto Components = Iter->GetComponents();
				for (auto SecondIter : Components)
				{
					auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
					if (!InterfacePtr)
					{
						continue;
					}
					auto AUDPtr = Cast<UDatasmithAssetUserData>(
					                                            InterfacePtr->GetAssetUserDataOfClass(
						                                             UDatasmithAssetUserData::StaticClass()
						                                            )
					                                           );
					if (!AUDPtr)
					{
						continue;
					}

					auto Datasmith_UniqueId = AUDPtr->MetaData.Find(TEXT("Datasmith_UniqueId"));
					if (!Datasmith_UniqueId)
					{
						continue;
					}

					auto MetaDataIter = AUDPtr->MetaData.Find(*SpaceInfo.Key);
					if (!MetaDataIter)
					{
						continue;
					}
					if (*MetaDataIter != SpaceInfo.Value)
					{
						continue;
					}

					auto SpaceNameValueIter = AUDPtr->MetaData.Find(*SpaceInfo.SpaceNameValue);
					if (!SpaceNameValueIter)
					{
						continue;
					}

					auto HashCode = HashCombine(
					                            GetTypeHash(*MetaDataIter),
					                            GetTypeHash(*SpaceNameValueIter)
					                           );

					if (MergeActorsMap.Contains(HashCode))
					{
						MergeActorsMap[HashCode]->Merge(Iter, {*SpaceInfo.Key, SpaceInfo.Value}, AUDPtr->MetaData);
					}
					else
					{
						auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_Space>(
							 UAssetRefMap::GetInstance()->
							 SceneElement_SpaceClass
							);
						NewActorPtr->Merge(Iter, {*SpaceInfo.Key, SpaceInfo.Value}, AUDPtr->MetaData);
						NewActorPtr->SceneElementID = *Datasmith_UniqueId;
						NewActorPtr->InitialSceneElement();

						MergeActorsMap.Add(HashCode, NewActorPtr);
					}
					break;
				}
			}
		}

		DSIter->Destroy();
	}
	AllReference.SpaceItemSet.DatasmithSceneActorSet.Empty();

	return true;
}

bool UGT_InitializeSceneActors::ProcessTask_Lanscape(
	ASceneElementCategory* SceneElementCategoryPtr,
	TSet<TSoftObjectPtr<ADatasmithSceneActor>>& DatasmithSceneActorSet
	)
{
	for (const auto& DSIter : DatasmithSceneActorSet)
	{
		DSIter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);

		TArray<AActor*> OutActors;

		DSIter.LoadSynchronous()->GetAttachedActors(OutActors, true, true);

		{
			TArray<AActor*> TempOutActors;

			DSIter.LoadSynchronous()->GetAttachedActors(TempOutActors, true);
			for (auto Iter : TempOutActors)
			{
				Iter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);
			}
		}

		for (auto& Iter : OutActors)
		{
			auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_LandScape>(
			                                                                  );
			NewActorPtr->Replace(Iter, {}, {});
			NewActorPtr->InitialSceneElement();
			NewActorPtr->QuitAllState();
		}

		DSIter->Destroy();
	}
	DatasmithSceneActorSet.Empty();

	return false;
}

bool UGT_InitializeSceneActors::ProcessTask_Generic(
	ASceneElementCategory* SceneElementCategoryPtr,
	FSceneElementMap& AllReference,
	FSceneElementFilter& StructItemSet
	)
{
	for (const auto& DSIter : StructItemSet.DatasmithSceneActorSet)
	{
		DSIter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);

		TArray<AActor*> OutActors;

		DSIter.LoadSynchronous()->GetAttachedActors(OutActors, true, true);

		{
			TArray<AActor*> TempOutActors;

			DSIter.LoadSynchronous()->GetAttachedActors(TempOutActors, true);
			for (auto Iter : TempOutActors)
			{
				Iter->AttachToActor(SceneElementCategoryPtr, FAttachmentTransformRules::KeepWorldTransform);
			}
		}

		TMap<int32, ASceneElementBase*> MergeActorsMap;
		for (auto& Iter : OutActors)
		{
#if WITH_EDITOR
			const auto SceneElementName = Iter->GetActorLabel();

			if (SceneElementName == TEXT("墙_幕墙_幕墙_58"))
			{
				// checkNoEntry();
			}
#endif // WITH_EDITOR

			if (!IsValid(Iter))
			{
				PRINTINVOKEINFO();
				continue;
			}

			bool bIsSceneElement = false;
			auto Components = Iter->GetComponents();
			for (auto SecondIter : Components)
			{
				auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
				if (!InterfacePtr)
				{
					continue;
				}
				auto AUDPtr = Cast<UDatasmithAssetUserData>(
				                                            InterfacePtr->GetAssetUserDataOfClass(
					                                             UDatasmithAssetUserData::StaticClass()
					                                            )
				                                           );
				if (!AUDPtr)
				{
					continue;
				}

				auto Datasmith_UniqueId = AUDPtr->MetaData.Find(TEXT("Datasmith_UniqueId"));
				if (!Datasmith_UniqueId)
				{
					continue;
				}

#if WITH_EDITOR
				if (*Datasmith_UniqueId == TEXT("df65c95dd1582c9fd02444023804e4dc"))
				{
					// checkNoEntry();
				}
#endif // WITH_EDITOR
#if WITH_EDITOR
				if (*Datasmith_UniqueId == TEXT("06d4996d50bad77b56bbd25a00cdd065"))
				{
					// checkNoEntry();
				}
#endif // WITH_EDITOR
				if (ProcessTask_Generic_SceneElementManaggerNeedMergeByUserData(
				                                                                AUDPtr,
				                                                                *Datasmith_UniqueId,
				                                                                Iter,
				                                                                MergeActorsMap
				                                                               ))
				{
					bIsSceneElement = true;
					break;
				}
				else if (ProcessTask_Generic_NeedReplaceByUserData(AUDPtr, *Datasmith_UniqueId, Iter))
				{
					bIsSceneElement = true;
					break;
				}
				else if (ProcessTask_Generic_NeedMergeByUserData(AUDPtr, *Datasmith_UniqueId, Iter, MergeActorsMap))
				{
					bIsSceneElement = true;
					break;
				}
			}

			if (bIsSceneElement)
			{
			}
			else
			{
				if (Iter->IsA(ASceneElementBase::StaticClass()))
				{
					continue;
				}

				if (Iter->IsA(AStaticMeshActor::StaticClass()))
				{
					auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_Regualar>(
						);
					NewActorPtr->Replace(Iter, {}, {});
					NewActorPtr->InitialSceneElement();
					NewActorPtr->QuitAllState();
				}
				else
				{
					auto NewActorPtr = GetWorld()->SpawnActor<ASceneElement_Regualar>(
						);
					NewActorPtr->Replace(Iter, {}, {});
					NewActorPtr->InitialSceneElement();
					NewActorPtr->QuitAllState();
				}
			}
		}

		DSIter->Destroy();
	}
	StructItemSet.DatasmithSceneActorSet.Empty();

	return false;
}

bool UGT_InitializeSceneActors::ProcessTask_Generic_NeedReplaceByUserData(
	UDatasmithAssetUserData* AUDPtr,
	const FString& Datasmith_UniqueId,
	AActor* ActorPtr
	)
{
	for (const auto& ThirdIter : UAssetRefMap::GetInstance()->NeedReplaceByUserData)
	{
		auto MetaDataIter = AUDPtr->MetaData.Find(*ThirdIter.Key.Key);
		if (!MetaDataIter)
		{
			continue;
		}
		if (ThirdIter.Key.bOnlyKey)
		{
		}
		else if (ThirdIter.Key.bSkip)
		{
			return true;
		}
		else
		{
			if (*MetaDataIter != ThirdIter.Key.Value)
			{
				continue;
			}
		}

		{
			auto NewActorPtr = GetWorld()->SpawnActor<ASceneElementBase>(
			                                                             ThirdIter.Value
			                                                            );
			NewActorPtr->Replace(ActorPtr, {*ThirdIter.Key.Key, *MetaDataIter}, AUDPtr->MetaData);
			NewActorPtr->SceneElementID = Datasmith_UniqueId;
			NewActorPtr->InitialSceneElement();
			NewActorPtr->QuitAllState();

			USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
			                                             Add(NewActorPtr->SceneElementID, NewActorPtr);
		}

		return true;
	}

	return false;
}

bool UGT_InitializeSceneActors::ProcessTask_Generic_NeedMergeByUserData(
	UDatasmithAssetUserData* AUDPtr,
	const FString& Datasmith_UniqueId,
	AActor* ActorPtr,
	TMap<int32, ASceneElementBase*>& MergeActorsMap
	)
{
	for (const auto& ThirdIter : UAssetRefMap::GetInstance()->NeedMergeByUserData)
	{
		auto MetaDataIter = AUDPtr->MetaData.Find(*ThirdIter.Key.Key);
		if (!MetaDataIter)
		{
			continue;
		}

		if (ThirdIter.Key.bOnlyKey)
		{
		}
		else
		{
			if (*MetaDataIter != ThirdIter.Key.Value)
			{
				continue;
			}
		}

		auto HashCode = 0;

		if (ThirdIter.Key.bIsRhinoMode)
		{
			if (ActorPtr->Tags.IsValidIndex(2))
			{
				HashCode = HashCombine(
				                       GetTypeHash(*MetaDataIter),
				                       GetTypeHash(ActorPtr->Tags[2]),
				                       GetTypeHash(ThirdIter.Key)
				                      );
			}
			else
			{
				checkNoEntry();
				return false;
			}
		}
		else
		{
			HashCode = HashCombine(
			                       GetTypeHash(*MetaDataIter),
			                       GetTypeHash(ThirdIter.Key)
			                      );
		}

		if (MergeActorsMap.Contains(HashCode))
		{
			MergeActorsMap[HashCode]->Merge(
			                                ActorPtr,
			                                {*ThirdIter.Key.Key, *MetaDataIter},
			                                AUDPtr->MetaData
			                               );
			MergeActorsMap[HashCode]->InitialSceneElement();
		}
		else
		{
			auto NewActorPtr = GetWorld()->SpawnActor<ASceneElementBase>(
			                                                             ThirdIter.Value
			                                                            );
			NewActorPtr->Merge(ActorPtr, {*ThirdIter.Key.Key, *MetaDataIter}, AUDPtr->MetaData);
			NewActorPtr->SceneElementID = *Datasmith_UniqueId;
			NewActorPtr->InitialSceneElement();
			if (ThirdIter.Value == ASceneElement_PWR_Pipe::StaticClass())
			{
			}
			NewActorPtr->QuitAllState();

			MergeActorsMap.Add(HashCode, NewActorPtr);
			USceneInteractionWorldSystem::GetInstance()->SceneElementMap.
			                                             Add(NewActorPtr->SceneElementID, NewActorPtr);
		}

		return true;
	}
	return false;
}

bool UGT_InitializeSceneActors::ProcessTask_Generic_SceneElementManaggerNeedMergeByUserData(
	UDatasmithAssetUserData* AUDPtr,
	const FString& Datasmith_UniqueId,
	AActor* ActorPtr,
	TMap<int32, ASceneElementBase*>& MergeActorsMap
	)
{
	for (const auto& ThirdIter : UAssetRefMap::GetInstance()->SceneElementManaggerNeedMergeByUserData)
	{
		for (const auto& FourIter : ThirdIter.Key.Maps)
		{
			auto MetaDataIter = AUDPtr->MetaData.Find(*FourIter.Key);
			if (!MetaDataIter)
			{
				continue;
			}

			for (const auto& FiveIter : FourIter.Value.StrAry)
			{
				if (*MetaDataIter == FiveIter)
				{
					auto HashCode = 0;

					HashCode = HashCombine(
					                       GetTypeHash(ThirdIter.Key)
					                      );

					TPair<FName, FString> InUserData(FourIter.Key, FiveIter);

					if (SceneElementManaggerMergeActorsMap.Contains(HashCode))
					{
						SceneElementManaggerMergeActorsMap[HashCode]->Merge(ActorPtr, InUserData, AUDPtr->MetaData);
						SceneElementManaggerMergeActorsMap[HashCode]->InitialSceneElement();
					}
					else
					{
						auto NewActorPtr = GetWorld()->SpawnActor<ASceneElementManaggerBase>(
							 ThirdIter.Value
							);
						NewActorPtr->Merge(ActorPtr, InUserData, AUDPtr->MetaData);
						NewActorPtr->SceneElementID = *Datasmith_UniqueId;
						NewActorPtr->InitialSceneElement();
						if (ThirdIter.Value == ASceneElement_PWR_Pipe::StaticClass())
						{
						}
						NewActorPtr->QuitAllState();

						SceneElementManaggerMergeActorsMap.Add(HashCode, NewActorPtr);
						USceneInteractionWorldSystem::GetInstance()->SceneElementManaggerMap.
						                                             Add(
						                                                 NewActorPtr->SceneElementID,
						                                                 NewActorPtr
						                                                );
					}

					return true;
				}
			}
		}
	}

	return false;
}
