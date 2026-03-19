// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "GameOptions.h"
#include "SceneElementBase.h"
#include "SceneElement_DeviceBase.h"
#include "SEJC.h"

#include "SceneElement_ControlBorder.generated.h"

class UPlayerComponent;
class USphereComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class AViewerPawn;
class UActorSequenceComponent;
class URouteMarker;
class UDatasmithAssetUserData;
class AFloorHelper;
class UBoxComponent;
class UDTMqttClient;

/**
 * 控制面板
 */
UCLASS()
class LAYERVISUALIZE_API ASceneElement_ControlBorder :
	public ASceneElement_DeviceBase
{
	GENERATED_BODY()

public:
	ASceneElement_ControlBorder(
		const FObjectInitializer& ObjectInitializer
		);

	virtual void BeginPlay() override;

	virtual FBox GetComponentsBoundingBox(
		bool bNonColliding = false,
		bool bIncludeFromChildActors = false
		) const override;

	virtual void InitialSceneElement() override;

	virtual void ReplaceImp(
		AActor* ActorPtr,
		const TPair<FName, FString>& InUserData
		) override;

	virtual void BeginInteraction() override;

	virtual void EndInteraction() override;

	virtual TMap<FString, FString> GetStateDescription() const override;

	virtual void SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSet<TObjectPtr<ASceneElementBase>> ControlDevicesSet;

protected:
	virtual void QuitAllState(
		) override;

	UFUNCTION()
	void Connected(
		const FString& Cause
		);

	UFUNCTION()
	void ConnectionLost(
		const FString& Cause
		);

	UFUNCTION()
	void MessageArrived(
		const FString& Topic,
		const FString& Message
		);

	bool bIsOpened = false;

	virtual void QueryDeviceInfoComplete(
		bool bSuccess,
		const FString& ResponStr
		) override;

	struct FAirData
	{
		void Deserialize(
			const FString& JsonStr
			);

		FString type;
		FString deviceId;
		FString version;
		FString code;
		FString userId;
		FString pairCnt;

		struct Fdevices
		{
			FString serialid;
			FString devType;
			FString devName;
			FString iotDeviceName;
			FString deviceSecret;
			FString iotId;

			struct Fchars
			{
				int32 charId;
				int32 charValue;
			};

			TArray<Fchars> chars;
		};

		TArray<Fdevices> devices;

		struct Fchar
		{
			int32 charId;
			int32 charValue;
			FString serialid;
			FString deviceId;
		};

		TArray<Fchar> charArray;
	};

	FAirData AirData;
	
	UPROPERTY()
	UDTMqttClient* DTMqttClientPtr = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMeshComponent*> StaticMeshComponentsAry;
};
