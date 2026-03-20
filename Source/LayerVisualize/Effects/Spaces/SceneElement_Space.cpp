#include "SceneElement_Space.h"

#include "AirQualityTips.h"
#include "Engine/OverlapResult.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"

#include "AssetRefMap.h"
#include "CollisionDataStruct.h"
#include "DatasmithAssetUserData.h"
#include "Dynamic_SkyBase.h"
#include "FeatureWheel.h"
#include "FloorHelper.h"
#include "MainHUD.h"
#include "MainHUDLayout.h"
#include "SceneElement_DeviceBase.h"
#include "Space_VolumetricFog.h"
#include "SmartCitySuiteTags.h"
#include "WeatherSystem.h"
#include "Algorithm.h"
#include "HttpModule.h"
#include "LogWriter.h"

ASceneElement_Space::ASceneElement_Space(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
}

void ASceneElement_Space::BeginPlay()
{
	Super::BeginPlay();

	auto Handle = UWeatherSystem::GetInstance()->GetDynamicSky()->OnHourChanged.AddCallback(
		 std::bind(
		           &ThisClass::OnHourChanged,
		           this,
		           std::placeholders::_1
		          )
		);
	Handle->bIsAutoUnregister = false;
}

FBox ASceneElement_Space::GetComponentsBoundingBox(
	bool bNonColliding,
	bool bIncludeFromChildActors
	) const
{
	FBox Box(ForceInit);

	for (auto Iter : CollisionComponentsAry)
	{
		Box += Iter->Bounds.GetBox();
	}

	return Box;
}

void ASceneElement_Space::InitialSceneElement()
{
	Super::InitialSceneElement();

	if (BelongFloor)
	{
		return;
	}

	auto ParentPtr = GetAttachParentActor();
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
		BelongFloor = FloorPtr;
	}

	GetWorldTimerManager().SetTimer(
	                                QueryDeviceTimerHandel,
	                                this,
	                                &ThisClass::QuerySpaceInfo,
	                                IntervalTime,
	                                true,
	                                FMath::RandRange(0.f, 30.f) // 避免拥挤
	                               );
}

void ASceneElement_Space::ReplaceImp(
	AActor* ActorPtr,
	const TPair<FName, FString>& InUserData
	)
{
}

void ASceneElement_Space::Merge(
	const TSoftObjectPtr<AActor>& ActorRef,
	const TPair<FName, FString>& InUserData,
	const TMap<FName, FString>& NewUserData
	)
{
	Super::Merge(ActorRef, InUserData, NewUserData);

	if (ActorRef.ToSoftObjectPath().IsValid())
	{
		AActor* ParentPtr = ActorRef->GetAttachParentActor();
		if (ParentPtr && !GetAttachParentActor())
		{
			AttachToActor(ParentPtr, FAttachmentTransformRules::KeepRelativeTransform);
			SetActorRelativeTransform(FTransform::Identity);
		}

		auto STPtr = Cast<AStaticMeshActor>(ActorRef.Get());
		if (STPtr)
		{
			TArray<UStaticMeshComponent*> Components;
			STPtr->GetComponents<UStaticMeshComponent>(Components);
			for (auto SecondIter : Components)
			{
				auto InterfacePtr = Cast<IInterface_AssetUserData>(SecondIter);
				if (InterfacePtr)
				{
					auto AUDPtr = Cast<UDatasmithAssetUserData>(
					                                            InterfacePtr->GetAssetUserDataOfClass(
						                                             UDatasmithAssetUserData::StaticClass()
						                                            )
					                                           );
					if (!AUDPtr)
					{
						continue;
					}
					for (const auto& ThirdIter : AUDPtr->MetaData)
					{
						if (ThirdIter.Key == DataSmith_Key)
						{
							Category = ThirdIter.Value;
							break;
						}
					}
					break;
				}
			}

			auto NewComponentPtr = Cast<UStaticMeshComponent>(
			                                                  AddComponentByClass(
				                                                   UStaticMeshComponent::StaticClass(),
				                                                   true,
				                                                   STPtr->GetStaticMeshComponent()->
				                                                          GetComponentTransform(),
				                                                   false
				                                                  )
			                                                 );

			NewComponentPtr->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());
			NewComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

			NewComponentPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			NewComponentPtr->SetRenderCustomDepth(false);

			auto SpaceMaterialInstance = UAssetRefMap::GetInstance()->SpaceMaterialInstance;

			auto MatInst = UMaterialInstanceDynamic::Create(SpaceMaterialInstance.LoadSynchronous(), this);

			for (int32 Index = 0; Index < NewComponentPtr->GetNumMaterials(); Index++)
			{
				NewComponentPtr->SetMaterial(Index, MatInst);
			}

			NewComponentPtr->SetCastShadow(false);
			NewComponentPtr->bVisibleInReflectionCaptures = false;
			NewComponentPtr->bVisibleInRealTimeSkyCaptures = false;
			NewComponentPtr->bVisibleInRayTracing = false;
			NewComponentPtr->bReceivesDecals = false;
			NewComponentPtr->bUseAsOccluder = false;

			StaticMeshComponentsAry.Add(NewComponentPtr);

			auto CollisionComponentPtr = Cast<UBoxComponent>(
			                                                 AddComponentByClass(
				                                                  UBoxComponent::StaticClass(),
				                                                  true,
				                                                  STPtr->GetStaticMeshComponent()->
				                                                         GetComponentTransform(),
				                                                  false
				                                                 )
			                                                );

			CollisionComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

			CollisionComponentPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CollisionComponentPtr->SetCollisionObjectType(Space_Object);
			CollisionComponentPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

			CollisionComponentPtr->SetCollisionResponseToChannel(ExternalWall_Object, ECollisionResponse::ECR_Overlap);
			CollisionComponentPtr->SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
			CollisionComponentPtr->SetCollisionResponseToChannel(Device_Object, ECollisionResponse::ECR_Overlap);

			FBox Box(ForceInit);
			STPtr->GetStaticMeshComponent()->GetLocalBounds(Box.Min, Box.Max);

			CollisionComponentPtr->SetRelativeLocation(
			                                           Box.GetCenter() + STPtr->GetStaticMeshComponent()->
			                                           GetComponentLocation()
			                                          );

			CollisionComponentPtr->SetBoxExtent(Box.GetExtent());

			CollisionComponentPtr->SetRenderCustomDepth(false);

			CollisionComponentsAry.Add(CollisionComponentPtr);

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

			if (!FloorPtr)
			{
				return;
			}

			if (!FloorPtr->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_Roof))
			{
				// auto LightComponentPtr = Cast<UPointLightComponent>(
				//                                                     AddComponentByClass(
				// 	                                                     UPointLightComponent::StaticClass(),
				// 	                                                     true,
				// 	                                                     STPtr->GetStaticMeshComponent()->
				// 	                                                     GetComponentTransform(),
				// 	                                                     false
				// 	                                                    )
				//                                                    );
				//
				// LightComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
				//
				// LightComponentPtr->SetRelativeLocation(Box.GetCenter());
				//
				// LightComponentPtr->bUseInverseSquaredFalloff = false;
				// LightComponentPtr->IntensityUnits = ELightUnits::Lumens;
				// const auto Size = Box.GetSize().Size();
				// LightComponentPtr->SourceRadius = Size;
				// LightComponentPtr->SoftSourceRadius = Size;
				// LightComponentPtr->SourceLength = Size;
				// LightComponentPtr->Intensity = 8;
				//
				// LightComponentPtr->SetRelativeLocation(Box.GetCenter());
				//
				// RectLightComponentAry.Add(LightComponentPtr);
			}
		}

		ActorRef->Destroy();
	}
}

void ASceneElement_Space::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	double InScopeSeconds = 0.;
	ON_SCOPE_EXIT
	{
		if (InScopeSeconds > 0.3)
		{
			checkNoEntry();
		}
		PRINTINVOKEWITHSTR(FString::Printf(TEXT("InScopeSeconds %.2lf"), InScopeSeconds));
	};

	FSimpleScopeSecondsCounter SimpleScopeSecondsCounter(InScopeSeconds);

	Super::SwitchInteractionType(ConditionalSet);

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) ||
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			QuitAllState();
			OnExternalWall();
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_View)
		)
		{
			EntryViewDevice(ConditionalSet);
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_Focus)
		)
		{
			EntryFocusDevice(ConditionalSet);
			return;
		}
	}
	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Mode_BatchControl)
		)
		{
			EntryShowEffect(ConditionalSet);

			return;
		}
	}
	{
		if (
			(ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
			 ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Interaction) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger)
		)
		{
			EntryShowEffect(ConditionalSet);
			return;
		}
	}
	{
		if (
			(ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
			 ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)) &&
			ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Mode_DeviceManagger)
		)
		{
			EntryShowEffect(ConditionalSet);

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			EntryShowEffect(ConditionalSet);

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

TSharedPtr<FJsonValue> ASceneElement_Space::GetSceneElementData() const
{
	auto Result = Super::GetSceneElementData();

	auto RootJsonObj = Result->AsObject();

	RootJsonObj->SetStringField(
	                            TEXT("DataSmith_Key"),
	                            DataSmith_Key
	                           );

	RootJsonObj->SetStringField(
	                            TEXT("Category"),
	                            Category
	                           );

	return Result;
}

void ASceneElement_Space::UpdateExtensionParamMap(
	const TMap<FString, FString>& NewExtensionParamMap,
	bool bImmediatelyUpdate
	)
{
	Super::UpdateExtensionParamMap(NewExtensionParamMap, bImmediatelyUpdate);

	ApplyExtensionParamMap();
}

void ASceneElement_Space::SetFeatures(
	const TArray<FString>& InFeaturesAry
	)
{
	FeaturesAry = InFeaturesAry;
}

TSet<ASceneElement_DeviceBase*> ASceneElement_Space::GetAllDevices() const
{
	TSet<ASceneElement_DeviceBase*> Result;

	TArray<FOverlapResult> OutOverlap;

	FComponentQueryParams Params;

	// Params.bTraceComplex = true;

#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	Params.bDebugQuery = true;
#endif

	FCollisionObjectQueryParams ObjectQueryParams;

	ObjectQueryParams.AddObjectTypesToQuery(Device_Object);

	for (auto MeshIter : CollisionComponentsAry)
	{
		const auto Collision = MeshIter->GetCollisionEnabled();
		MeshIter->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetWorld()->ComponentOverlapMulti(
		                                  OutOverlap,
		                                  MeshIter,
		                                  // FVector::ZeroVector,
		                                  // FRotator::ZeroRotator,
		                                  MeshIter->GetComponentLocation(),
		                                  MeshIter->GetComponentRotation(),
		                                  Params,
		                                  ObjectQueryParams
		                                 );
		MeshIter->SetCollisionEnabled(Collision);

		for (const auto& Iter : OutOverlap)
		{
			if (Iter.GetActor() && !Iter.GetActor()->IsHidden())
			{
				auto DevicePtr = Cast<ASceneElement_DeviceBase>(Iter.GetActor());
				if (DevicePtr)
				{
					Result.Add(DevicePtr);
				}
			}
		}
	}

	return Result;
}

void ASceneElement_Space::UpdateSpaceDeviceData(
	const FString& DataType,
	const FString& DeviceID,
	int32 Value
	)
{
	if (Values.Contains(DataType))
	{
		Values[DataType].Add(DeviceID, Value);
	}
	else
	{
		auto& Ref = Values.Add(DataType);

		Ref.Add(DeviceID, Value);
	}
}

void ASceneElement_Space::RemoveSpaceDeviceData(
	const FString& DataType,
	ASceneElement_DeviceBase* SceneElement_DevicePtr
	)
{
	if (!SceneElement_DevicePtr)
	{
		return;
	}

	Values.Remove(DataType);
}

void ASceneElement_Space::EntryViewDevice(
	const FSceneElementConditional& ConditionalSet
	)
{
	SetActorHiddenInGame(false);

	for (auto Iter : FeatureWheelAry)
	{
		if (Iter)
		{
			Iter->RemoveFromParent();
		}
	}

	FeatureWheelAry.Empty();

	for (auto PrimitiveComponentPtr : StaticMeshComponentsAry)
	{
		if (PrimitiveComponentPtr)
		{
			PrimitiveComponentPtr->SetHiddenInGame(false);
			PrimitiveComponentPtr->SetRenderInMainPass(false);
			PrimitiveComponentPtr->SetRenderCustomDepth(true);
			PrimitiveComponentPtr->SetCustomDepthStencilValue(UGameOptions::GetInstance()->FocusOutline);
		}
	}

	for (auto Iter : CollisionComponentsAry)
	{
		Iter->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ShowFog();
}

void ASceneElement_Space::EntryFocusDevice(
	const FSceneElementConditional& ConditionalSet
	)
{
}

void ASceneElement_Space::EntryShow(
	const FSceneElementConditional& ConditionalSet
	)
{
	SetActorHiddenInGame(true);

	for (auto PrimitiveComponentPtr : StaticMeshComponentsAry)
	{
		if (PrimitiveComponentPtr)
		{
			PrimitiveComponentPtr->SetHiddenInGame(false);
			PrimitiveComponentPtr->SetRenderInMainPass(false);
			PrimitiveComponentPtr->SetRenderCustomDepth(false);
		}
	}

	for (auto Iter : CollisionComponentsAry)
	{
		Iter->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	ClearFog();
}

void ASceneElement_Space::EntryShowEffect(
	const FSceneElementConditional& ConditionalSet
	)
{
}

void ASceneElement_Space::QuitAllState()
{
	Super::QuitAllState();

	SetActorHiddenInGame(false);

	for (auto Iter : FeatureWheelAry)
	{
		if (Iter)
		{
			Iter->RemoveFromParent();
		}
	}
	FeatureWheelAry.Empty();

	for (auto PrimitiveComponentPtr : StaticMeshComponentsAry)
	{
		if (PrimitiveComponentPtr)
		{
			PrimitiveComponentPtr->SetHiddenInGame(true);
			PrimitiveComponentPtr->SetRenderCustomDepth(false);
		}
	}

	for (auto Iter : CollisionComponentsAry)
	{
		Iter->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ClearFog();
}

void ASceneElement_Space::SwitchColor(
	const FColor& Color
	)
{
}

void ASceneElement_Space::OnClickedTag()
{
}

void ASceneElement_Space::OnHourChanged(
	int32 Hour
	)
{
	for (auto Iter : RectLightComponentAry)
	{
		Iter->SetHiddenInGame(true);
	}
}

void ASceneElement_Space::OnExternalWall()
{
	const auto Hour = UWeatherSystem::GetInstance()->GetDynamicSky()->GetCurrentHour();

	if (Hour > 18 || Hour < 8)
	{
		for (auto Iter : RectLightComponentAry)
		{
			Iter->SetHiddenInGame(false);
		}
		return;
	}
	for (auto Iter : RectLightComponentAry)
	{
		Iter->SetHiddenInGame(true);
		DrawDebugSphere(GetWorld(), Iter->GetComponentLocation(), 20, 20, FColor::Red, false, 10);
	}
}

void ASceneElement_Space::ShowFog()
{
	if (VolumetricFogAry.IsEmpty())
	{
		FActorSpawnParameters SpawnParameters;

		for (auto MeshIter : CollisionComponentsAry)
		{
			SpawnParameters.CustomPreSpawnInitalization = [MeshIter](
				AActor* Actor
				)
				{
					auto FogPtr = Cast<ASpace_VolumetricFog>(Actor);
					if (FogPtr)
					{
						FogPtr->SetBoxSize(MeshIter->GetScaledBoxExtent() * 2);
					}
				};

			VolumetricFogAry.Add(
			                     GetWorld()->SpawnActor<ASpace_VolumetricFog>(
			                                                                  UAssetRefMap::GetInstance()->
			                                                                  SceneElement_Space_VolumetricFogClass,
			                                                                  MeshIter->GetComponentLocation(),
			                                                                  FRotator::ZeroRotator,
			                                                                  SpawnParameters
			                                                                 )
			                    );
		}
	}
	else
	{
	}

	ApplyExtensionParamMap();
}

void ASceneElement_Space::ClearFog()
{
	for (auto Iter : VolumetricFogAry)
	{
		if (Iter)
		{
			Iter->Destroy();
		}
	}

	VolumetricFogAry.Empty();

	if (AirQualityTips)
	{
		AirQualityTips->RemoveFromParent();
	}
	AirQualityTips = nullptr;
}

void ASceneElement_Space::ApplyExtensionParamMap()
{
	UpdateSpaceDeviceDataImp(TEXT("空气质量"));
}

void ASceneElement_Space::UpdateSpaceDeviceDataImp(
	const FString& DataType
	)
{
#if TEST_AIRQUALITY
	Values.Add(DataType, {{TEXT(""), FMath::RandRange(0, 300)}});
#endif

	if (Values.Contains(DataType))
	{
		float Value = 0.f;
		const auto& Ref = Values[DataType];
		for (const auto& Iter : Ref)
		{
			Value += Iter.Value;
		}

		Value = Value / Ref.Num();
		Value = FMath::Clamp(Value, 0.0f, 251.0f);

		if (Value > 0)
		{
			const auto FV = FMath::Lerp(0.f, 1.5f, Value / 251.f);

			for (auto Iter : VolumetricFogAry)
			{
				if (Iter)
				{
					Iter->SetValue(FV);
				}
			}

			if (Value > 150)
			{
				if (AirQualityTips)
				{
				}
				else
				{
					AirQualityTips = CreateWidget<UAirQualityTips>(
					                                               GetWorld(),
					                                               UAssetRefMap::GetInstance()->AirQualityTipsClass
					                                              );
					AirQualityTips->AddToViewport();

					auto TargetPt = UKismetAlgorithm::GetActorBox(
					                                              {this}
					                                             );

					AirQualityTips->TargetPt = TargetPt.GetCenter();
				}

				if (AirQualityTips)
				{
					AirQualityTips->UpdateValue(Value);
				}
			}

			return;
		}
	}
	else
	{
		for (auto Iter : VolumetricFogAry)
		{
			if (Iter)
			{
				Iter->SetValue(0);
			}
		}
	}

	if (AirQualityTips)
	{
		AirQualityTips->RemoveFromParent();
	}
	AirQualityTips = nullptr;
}

void ASceneElement_Space::QuerySpaceInfo()
{
}

void ASceneElement_Space::QueryDeviceInfoComplete(
	bool bSuccess,
	const FString& ResponStr
	)
{
	QueryDeviceInfo_.Deserialize(ResponStr);

}

void ASceneElement_Space::QuerySpaceInfoComplete(
	bool bSuccess,
	const FString& ResponStr
	)
{
	QuerySpaceInfo_.Deserialize(ResponStr);

	for (const auto Iter : RequestAry)
	{
		Iter->CancelRequest();
	}
	RequestAry.Empty();
}

void ASceneElement_Space::Connected(
	const FString& Cause
	)
{
}

void ASceneElement_Space::ConnectionLost(
	const FString& Cause
	)
{
}

void ASceneElement_Space::MessageArrived(
	const FString& Topic,
	const FString& Message
	)
{
	FAirData AirData;

	AirData.Deserialize(Message);

	if (AirData.type == TEXT("ACCESSORY"))
	{
		for (const auto& Iter : AirData.charArray)
		{
			if (Iter.charId == 818)
			{
			}
			if (Iter.charId == 819)
			{
				UpdateSpaceDeviceData(TEXT("空气质量"), Iter.deviceId, Iter.charValue);

				return;
			}
		}
	}
	else if (AirData.type == TEXT("GetAllDeviceInfo"))
	{
		for (const auto& Iter : AirData.devices)
		{
			for (const auto& SecondIter : Iter.chars)
			{
				if (SecondIter.charId == 818)
				{
				}
				if (SecondIter.charId == 819)
				{
					UpdateSpaceDeviceData(TEXT("空气质量"), AirData.deviceId, SecondIter.charValue);

					return;
				}
			}
		}
	}
}

void ASceneElement_Space::FQuerySpaceInfo::Deserialize(
	const FString& JsonStr
	)
{
	TSharedRef<TJsonReader<>> JsonReader =
		TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                            );

	if (jsonObject)
	{
	}
	else
	{
		return;
	}

	if (jsonObject->TryGetStringField(
	                                  TEXT("message"),
	                                  message
	                                 ))
	{
	}

	const TArray<TSharedPtr<FJsonValue>>* body_OutObjectAry = nullptr;
	if (jsonObject->TryGetArrayField(
	                                 TEXT("body"),
	                                 body_OutObjectAry
	                                ))
	{
		const TArray<TSharedPtr<FJsonValue>>* bimModelDeviceAssociations_OutArray = nullptr;
		body_Ary.Empty();

		for (auto Iter : *body_OutObjectAry)
		{
			FbimModelDeviceAssociations bimModelDeviceAssociations;
			auto ObjSPtr = Iter->AsObject();

			if (ObjSPtr->TryGetNumberField(
			                               TEXT("id"),
			                               bimModelDeviceAssociations.id
			                              ))
			{
			}

			if (ObjSPtr->TryGetStringField(
			                               TEXT("status"),
			                               bimModelDeviceAssociations.status
			                              ))
			{
			}

			if (ObjSPtr->TryGetStringField(
			                               TEXT("bimId"),
			                               bimModelDeviceAssociations.bimId
			                              ))
			{
			}

			body_Ary.Add(bimModelDeviceAssociations);
		}
	}
}

void ASceneElement_Space::FQueryDeviceInfo::Deserialize(
	const FString& JsonStr
	)
{
	TSharedRef<TJsonReader<>> JsonReader =
		TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                            );

	if (jsonObject)
	{
	}
	else
	{
		return;
	}

	if (jsonObject->TryGetStringField(
	                                  TEXT("message"),
	                                  message
	                                 ))
	{
	}

	const TSharedPtr<FJsonObject>* body_OutObject = nullptr;
	if (jsonObject->TryGetObjectField(
	                                  TEXT("body"),
	                                  body_OutObject
	                                 ))
	{
		(*body_OutObject)->TryGetStringField(
		                                     TEXT("total"),
		                                     body_total
		                                    );

		const TArray<TSharedPtr<FJsonValue>>* bimModelDeviceAssociations_OutArray = nullptr;
		if ((*body_OutObject)->TryGetArrayField(
		                                        TEXT("bimModelDeviceAssociations"),
		                                        bimModelDeviceAssociations_OutArray
		                                       ))
		{
			body_Ary.Empty();

			for (auto Iter : *bimModelDeviceAssociations_OutArray)
			{
				FbimModelDeviceAssociations bimModelDeviceAssociations;
				auto ObjSPtr = Iter->AsObject();

				if (ObjSPtr->TryGetNumberField(
				                               TEXT("id"),
				                               bimModelDeviceAssociations.id
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("status"),
				                               bimModelDeviceAssociations.status
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("deviceId"),
				                               bimModelDeviceAssociations.deviceId
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("supplier"),
				                               bimModelDeviceAssociations.supplier
				                              ))
				{
				}
				body_Ary.Add(bimModelDeviceAssociations);
			}
		}
	}
}

void ASceneElement_Space::FAirData::Deserialize(
	const FString& JsonStr
	)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                            );

	if (!jsonObject)
	{
		return;
	}
	if (jsonObject->TryGetStringField(TEXT("type"), type))
	{
	}
	const TArray<TSharedPtr<FJsonValue>>* devices_Array;
	if (jsonObject->TryGetArrayField(TEXT("devices"), devices_Array))
	{
		devices.Empty();
		for (auto Iter : *devices_Array)
		{
			Fdevices device;
			auto ObjSPtr = Iter->AsObject();

			if (ObjSPtr->TryGetStringField(TEXT("serialid"), device.serialid))
			{
				const TArray<TSharedPtr<FJsonValue>>* chars_Array;
				if (ObjSPtr->TryGetArrayField(TEXT("chars"), chars_Array))
				{
					for (auto Second_Iter : *chars_Array)
					{
						Fdevices::Fchars char_;

						auto Second_ObjSPtr = Second_Iter->AsObject();

						if (Second_ObjSPtr->TryGetNumberField(TEXT("charId"), char_.charId))
						{
						}

						if (Second_ObjSPtr->TryGetNumberField(TEXT("charValue"), char_.charValue))
						{
						}

						device.chars.Add(char_);
					}
				}
			}

			devices.Add(device);
		}
	}
	const TArray<TSharedPtr<FJsonValue>>* charArray_Array;
	if (jsonObject->TryGetArrayField(TEXT("charArray"), charArray_Array))
	{
		charArray.Empty();
		for (auto Iter : *charArray_Array)
		{
			Fchar char_;
			auto ObjSPtr = Iter->AsObject();

			if (ObjSPtr->TryGetNumberField(TEXT("charId"), char_.charId))
			{
			}

			if (ObjSPtr->TryGetNumberField(TEXT("charValue"), char_.charValue))
			{
			}

			if (ObjSPtr->TryGetStringField(TEXT("serialid"), char_.serialid))
			{
			}

			if (ObjSPtr->TryGetStringField(TEXT("serialid"), char_.deviceId))
			{
			}

			charArray.Add(char_);
		}
	}
}
