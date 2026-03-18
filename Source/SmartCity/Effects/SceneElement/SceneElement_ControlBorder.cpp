#include "SceneElement_ControlBorder.h"

#include "ActorSequenceComponent.h"
#include "DatasmithAssetUserData.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"

#include "AssetRefMap.h"
#include "CollisionDataStruct.h"
#include "FloorHelper.h"
#include "GameplayTagsLibrary.h"
#include "MessageBody.h"
#include "RouteMarker.h"
#include "SmartCitySuiteTags.h"
#include "WebChannelWorldSystem.h"
#include "DTMqttClient.h"
#include "SceneElement_Space.h"

ASceneElement_ControlBorder::ASceneElement_ControlBorder(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	CollisionComponentHelper->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponentHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponentHelper->SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);
	CollisionComponentHelper->SetCollisionResponseToChannel(Device_Object, ECollisionResponse::ECR_Overlap);
	CollisionComponentHelper->SetCollisionObjectType(Device_Object);
}

void ASceneElement_ControlBorder::BeginPlay()
{
	Super::BeginPlay();
}

FBox ASceneElement_ControlBorder::GetComponentsBoundingBox(
	bool bNonColliding,
	bool bIncludeFromChildActors
	) const
{
	FBox Box(ForceInit);

	ForEachComponent<UPrimitiveComponent>(
	                                      bIncludeFromChildActors,
	                                      [&](
	                                      const UPrimitiveComponent* InPrimComp
	                                      )
	                                      {
		                                      if (StaticMeshComponent->GetStaticMesh())
		                                      {
			                                      // Only use collidable components to find collision bounding box.
			                                      if (InPrimComp->IsRegistered() && (
				                                          bNonColliding || InPrimComp->IsCollisionEnabled()))
			                                      {
				                                      Box += InPrimComp->Bounds.GetBox();
			                                      }
		                                      }
	                                      }
	                                     );

	return Box;
}

void ASceneElement_ControlBorder::InitialSceneElement()
{
	Super::InitialSceneElement();

	UDTMqttClient::CreateMqttClient(DTMqttClientPtr);

	DTMqttClientPtr->OnConnected.AddDynamic(this, &ThisClass::Connected);
	DTMqttClientPtr->OnConnectionLost.AddDynamic(this, &ThisClass::ConnectionLost);
	DTMqttClientPtr->OnMessageArrived.AddDynamic(this, &ThisClass::MessageArrived);

	FString MQTTAddress;

	 auto Path = FPaths::ProjectContentDir() / TEXT("Configs") / TEXT("RuntimeConfig.ini");
	Path = FConfigCacheIni::NormalizeConfigIniPath(Path);
	GConfig->GetString(
					   TEXT("SmartCitySetting"),
					   TEXT("MQTTAddress"),
					   MQTTAddress,
					   Path
					  );
	
	bool Success = false;
	FString ErrorMsg;
	// DTMqttClientPtr->Connect(
	// 						 FString::Printf(TEXT("ws://%s/mqtt"), *MQTTAddress),
	// 						 TEXT(""),
	// 						 TEXT(""),
	// 						 TEXT(""),
	// 						 60,
	// 						 5,
	// 						 Success,
	// 						 ErrorMsg
	// 						);
	//
	// if (Success)
	// {
	// }
}

void ASceneElement_ControlBorder::ReplaceImp(
	AActor* ActorPtr,
	const TPair<FName, FString>& InUserData
	)
{
	Super::ReplaceImp(ActorPtr, InUserData);

	if (ActorPtr)
	{
		if (ActorPtr->IsA(AStaticMeshActor::StaticClass()))
		{
			auto STPtr = Cast<AStaticMeshActor>(ActorPtr);
			if (STPtr)
			{
				auto InterfacePtr = Cast<IInterface_AssetUserData>(STPtr->GetStaticMeshComponent());
				if (!InterfacePtr)
				{
					return;
				}
				auto AUDPtr = Cast<UDatasmithAssetUserData>(
				                                            InterfacePtr->GetAssetUserDataOfClass(
					                                             UDatasmithAssetUserData::StaticClass()
					                                            )
				                                           );

				CheckIsJiaCeng(AUDPtr);

				StaticMeshComponent->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());

				for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
				{
					StaticMeshComponent->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
				}

				StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				FBox Box(ForceInit);
				StaticMeshComponent->GetLocalBounds(Box.Min, Box.Max);

				CollisionComponentHelper->SetRelativeLocation(Box.GetCenter());

				CollisionComponentHelper->SetBoxExtent(Box.GetExtent());
			}
		}
		else if (ActorPtr->IsA(AActor::StaticClass()))
		{
			TArray<UStaticMeshComponent*> STCAry;
			ActorPtr->GetComponents<UStaticMeshComponent>(STCAry);
			for (auto Iter : STCAry)
			{
				auto NewComponentPtr = Cast<UStaticMeshComponent>(
				                                                  AddComponentByClass(
					                                                   UStaticMeshComponent::StaticClass(),
					                                                   true,
					                                                   Iter->GetComponentTransform(),
					                                                   false
					                                                  )
				                                                 );

				NewComponentPtr->SetStaticMesh(Iter->GetStaticMesh());

				for (int32 Index = 0; Index < Iter->GetNumMaterials(); Index++)
				{
					StaticMeshComponent->SetMaterial(Index, Iter->GetMaterial(Index));
				}

				NewComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

				NewComponentPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				NewComponentPtr->SetCollisionObjectType(Device_Object);
				NewComponentPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

				NewComponentPtr->SetCollisionResponseToChannel(ExternalWall_Object, ECollisionResponse::ECR_Overlap);
				NewComponentPtr->SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
				NewComponentPtr->SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);

				NewComponentPtr->SetRenderCustomDepth(false);

				StaticMeshComponentsAry.Add(NewComponentPtr);
			}
		}
	}
}

void ASceneElement_ControlBorder::BeginInteraction()
{
	Super::BeginInteraction();

	bIsOpened = true;
}

void ASceneElement_ControlBorder::EndInteraction()
{
	Super::EndInteraction();

	bIsOpened = false;
}

TMap<FString, FString> ASceneElement_ControlBorder::GetStateDescription() const
{
	TMap<FString, FString> Result;

	if (bIsOpened)
	{
		Result.Add(TEXT(""), TEXT("开启"));
	}
	else
	{
		Result.Add(TEXT(""), TEXT("关闭"));
	}

	return Result;
}

void ASceneElement_ControlBorder::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);

	for (auto Iter : ControlDevicesSet)
	{
		if (Iter)
		{
			Iter->UpdateInteractionType(ConditionalSet);
		}
	}

	if (ProcessJiaCengLogic(ConditionalSet))
	{
		SetActorHiddenInGame(true);
		return;
	}

	{
		if (
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_ExternalWall) ||
			ConditionalSet.ConditionalSet.HasTagExact(USmartCitySuiteTags::Interaction_Area_Periphery)
		)
		{
			if (BelongFloor && BelongFloor->FloorTag.MatchesTag(USmartCitySuiteTags::Interaction_Area_Floor_Roof))
			{
				SetActorHiddenInGame(false);

				auto PrimitiveComponentPtr = GetComponentByClass<UPrimitiveComponent>();
				if (PrimitiveComponentPtr)
				{
					PrimitiveComponentPtr->SetRenderCustomDepth(false);
				}
			}
			else
			{
				QuitAllState();
			}

			return;
		}
	}
	{
		if ((ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Floor) ||
		     ConditionalSet.ConditionalSet.HasTag(USmartCitySuiteTags::Interaction_Area_Space)))
		{
			SetActorHiddenInGame(false);

			auto PrimitiveComponentPtr = GetComponentByClass<UPrimitiveComponent>();
			if (PrimitiveComponentPtr)
			{
				PrimitiveComponentPtr->SetRenderCustomDepth(false);
			}

			if (ExtensionParamMap.Contains(TEXT("空气质量")))
			{
				const auto Value = UKismetStringLibrary::Conv_StringToInt(ExtensionParamMap[TEXT("空气质量")]);
				if (Value)
				{
					const auto FV = Value / 251.f;
				}
			}

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

void ASceneElement_ControlBorder::QuitAllState()
{
	Super::QuitAllState();

	SetActorHiddenInGame(true);

	auto PrimitiveComponentPtr = GetComponentByClass<UPrimitiveComponent>();
	if (PrimitiveComponentPtr)
	{
		PrimitiveComponentPtr->SetRenderCustomDepth(false);
	}
}

void ASceneElement_ControlBorder::Connected(
	const FString& Cause
	)
{
	// DeviceRealID = TEXT("0001-0108-2523-0009");
	
	bool Success = false;
	FString ErrorMsg;
	DTMqttClientPtr->Subscribe(
	                           FString::Printf(TEXT("enno_p2_product/p2/android/%s/#"), *DeviceRealID),
	                           EDT_QualityOfService::QoS0,
	                           Success,
	                           ErrorMsg
	                          );
	if (Success)
	{
	}

	DTMqttClientPtr->Publish(
	                         FString::Printf(TEXT("enno_p2_product/p2/panno/%s"), *DeviceRealID),
	                         FString::Printf(
	                                         TEXT(R"({"type":"GetAllDeviceInfo","time":"2025-11-04 11:06:20","%s"})"),
	                                         *DeviceRealID
	                                        ),
	                         EDT_QualityOfService::QoS0,
	                         true,
	                         Success,
	                         ErrorMsg
	                        );
	if (Success)
	{
	}
}

void ASceneElement_ControlBorder::ConnectionLost(
	const FString& Cause
	)
{
	DTMqttClientPtr = nullptr;
}

void ASceneElement_ControlBorder::MessageArrived(
	const FString& Topic,
	const FString& Message
	)
{
	if (!SceneElement_SpacePtr)
	{
		return;
	}

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
			
					return;
				}
			}
		}
	}
}

void ASceneElement_ControlBorder::QueryDeviceInfoComplete(
	bool bSuccess,
	const FString& ResponStr
	)
{
	Super::QueryDeviceInfoComplete(bSuccess, ResponStr);

	if (bSuccess)
	{
		if (DTMqttClientPtr)
		{
		
		}
		else
		{
			UDTMqttClient::CreateMqttClient(DTMqttClientPtr);

			DTMqttClientPtr->OnConnected.AddDynamic(this, &ThisClass::Connected);
			DTMqttClientPtr->OnConnectionLost.AddDynamic(this, &ThisClass::ConnectionLost);
			DTMqttClientPtr->OnMessageArrived.AddDynamic(this, &ThisClass::MessageArrived);

			FString MQTTAddress;
	
			const auto Path = FPaths::ProjectContentDir() / TEXT("Configs") / TEXT("RuntimeConfig.ini");
			FConfigCacheIni::NormalizeConfigIniPath(Path);
			GConfig->GetString(
							   TEXT("SmartCitySetting"),
							   TEXT("MQTTAddress"),
							   MQTTAddress,
							   Path
							  );

			bool Success = false;
			FString ErrorMsg;
			DTMqttClientPtr->Connect(
									 FString::Printf(TEXT("ws://%s/mqtt"), *MQTTAddress),
									 TEXT(""),
									 TEXT(""),
									 TEXT(""),
									 60,
									 5,
									 Success,
									 ErrorMsg
									);

			if (Success)
			{
			}
		}
	}
	else
	{
		if (DTMqttClientPtr)
		{
			bool Success = false;
			FString ErrorMsg;
			DTMqttClientPtr->Disconnect(Success, ErrorMsg);
		}
		else
		{
		}

		DTMqttClientPtr = nullptr;
	}
}

void ASceneElement_ControlBorder::FAirData::Deserialize(
	const FString& JsonStr
	)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                            );

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
