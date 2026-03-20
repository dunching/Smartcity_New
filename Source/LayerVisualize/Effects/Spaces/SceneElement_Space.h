// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "SceneElementBase.h"

#include "SceneElement_Space.generated.h"

class UBoxComponent;
class UPlayerComponent;
class USphereComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class AViewerPawnBase;
class URouteMarker;
class ASceneElement_DeviceBase;
class UFeatureWheel;
class AFloorHelper;
class ULocalLightComponent;
class ASpace_VolumetricFog;
class UDTMqttClient;
class IHttpRequest;
class UAirQualityTips;

/**
 * 雷达扫描效果
 */
UCLASS()
class LAYERVISUALIZE_API ASceneElement_Space :
	public ASceneElementBase
{
	GENERATED_BODY()

public:
	ASceneElement_Space(
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

	virtual void Merge(
		const TSoftObjectPtr<AActor>& ActorRef,
		const TPair<FName, FString>& InUserData,
		const TMap<FName, FString>& NewUserData
		) override;

	virtual void SwitchInteractionType(
		const FSceneElementConditional& ConditionalSet
		) override;

	virtual TSharedPtr<FJsonValue> GetSceneElementData() const override;

	virtual void UpdateExtensionParamMap(
		const TMap<FString, FString>& NewExtensionParamMap,
		bool bImmediatelyUpdate
		) override;

	void SetFeatures(
		const TArray<FString>& InFeaturesAry
		);

	TSet<ASceneElement_DeviceBase*> GetAllDevices() const;

	void UpdateSpaceDeviceData(
		const FString&DataType,
		const FString& DeviceID,
		int32 Value
		);

	void RemoveSpaceDeviceData(
		const FString&DataType,
		ASceneElement_DeviceBase*SceneElement_DevicePtr
		);

	FString Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DataSmith_Key = TEXT("Element*空间划分");

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<UBoxComponent*> CollisionComponentsAry;

protected:
	virtual void EntryViewDevice(
		const FSceneElementConditional& ConditionalSet
		);

	virtual void EntryFocusDevice(
		const FSceneElementConditional& ConditionalSet
		);

	virtual void EntryShow(
		const FSceneElementConditional& ConditionalSet
		);

	virtual void EntryShowEffect(
		const FSceneElementConditional& ConditionalSet
		);
public:
	virtual void QuitAllState(
		) override;

	void SwitchColor(
		const FColor& Color
		);

	UFUNCTION()
	void OnClickedTag();

	void OnHourChanged(
		int32 Hour
		);

	void OnExternalWall();

	void ShowFog();

	void ClearFog();

	void ApplyExtensionParamMap();
	
	void UpdateSpaceDeviceDataImp(
		const FString&DataType
		);
	
	virtual void QueryDeviceInfoComplete(
		bool bSuccess,
		const FString& ResponStr
		);

	void QuerySpaceInfo();

	virtual void QuerySpaceInfoComplete(
		bool bSuccess,
		const FString& ResponStr
		);

	UFUNCTION()
	void Connected( const FString& Cause);
	
	UFUNCTION()
	void ConnectionLost( const FString& Cause);
	
	UFUNCTION()
	void MessageArrived(const FString& Topic, const FString& Message);
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<UStaticMeshComponent*> StaticMeshComponentsAry;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TObjectPtr<UFeatureWheel>> FeatureWheelAry;

	TArray<FString> FeaturesAry;

	UPROPERTY(Transient)
	UAirQualityTips* AirQualityTips = nullptr;

	TArray<TObjectPtr<ASpace_VolumetricFog>> VolumetricFogAry;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AFloorHelper> BelongFloor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ULocalLightComponent>> RectLightComponentAry;

	/**
	 * Key 数据类型
	 * Valeu ：Key 设备ID Value 值
	 */
	TMap<FString, TMap<FString, int32>> Values;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float IntervalTime = 10.f;

	struct FQuerySpaceInfo
	{
		FString message;

		struct FbimModelDeviceAssociations
		{
			int32 id = 0;

			FString status;

			FString bimId;
		};

		TArray<FbimModelDeviceAssociations>body_Ary;
		
		void Deserialize(
			const FString& JsonStr
			);
	};

	FQuerySpaceInfo QuerySpaceInfo_;
	
	struct FQueryDeviceInfo
	{
		FString message;

		FString body_total;

		struct FbimModelDeviceAssociations
		{
			int32 id = 0;

			FString status;

			FString deviceId;

			FString supplier;
		};

		TArray<FbimModelDeviceAssociations>body_Ary;
		
		void Deserialize(
			const FString& JsonStr
			);
	};

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

	FQueryDeviceInfo QueryDeviceInfo_;
	
	FTimerHandle QueryDeviceTimerHandel;

private:
	
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> SpaceInfoRequest;

	TArray<TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>>RequestAry;
};
