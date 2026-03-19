// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <atomic>
#include <mutex>

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "Interfaces/IHttpRequest.h"

#include "QueryDeviceInfoSubSystem.generated.h"

class UDTMqttClient;

/**
 * 
 */
UCLASS()
class LAYERDATA_API UQueryDeviceInfoSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UQueryDeviceInfoSubSystem* GetInstance();

	void StartUpdateMQTT();

	void StartQueryEnergyInfo();

	void StartQueryEnergyValue();

	struct FQuerySpaceInfo
	{
		FString message;

		struct FbimModelDeviceAssociations
		{
			int32 id = 0;

			FString status;

			FString bimId;

			FString floor;
		};

		/**
		 * bimID, 数据
		 */
		TMap<FString, FbimModelDeviceAssociations> body_Ary;

		void Deserialize(
			const FString& JsonStr
			);
	};

	FQuerySpaceInfo QuerySpaceInfo_;

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> QuerySpaceInfo();

	void QuerySpaceInfoComplete(
		FHttpRequestPtr Req,
		FHttpResponsePtr Resp,
		bool bSuccess
		);

	struct FQueryDeviceInfo
	{
		FString message;

		FString body_total;

		struct FbimModelDeviceAssociations
		{
			int32 id = 0;

			FString status;

			FString bimId;

			FString deviceId;

			FString serialid;

			int32 spaceId;

			FString supplier;
		};

		/**
		 * bimID、DeviceID
		 */
		TMap<FString, FString> BimIDMap;

		/**
		 * DeviceID、bimID、serialid
		 */
		TMap<FString, TMap<FString, FbimModelDeviceAssociations>> DeviceMap;

		void Deserialize(
			const FString& JsonStr
			);
	};

	FQueryDeviceInfo AllDeviceInfo;

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> QueryAllSpaceDeviceInfo(
		int32 Index = 1
		);

	void QueryAllSpaceDeviceInfoComplete(
		FHttpRequestPtr Req,
		FHttpResponsePtr Resp,
		bool bSuccess,
		int32 Index 
		);

	struct FEnergyInfo
	{
		FString message;

		struct FbimModelDeviceAssociations
		{
			int32 id = 0;

			FString status;

			FString bimId;

			FString deviceId;

			FString serialid;

			float Value = 0.f;
		};

		/**
		 * BimID, 数据
		 */
		TMap<FString, TSharedPtr<FbimModelDeviceAssociations>> BimIDMap;

		void Deserialize(
			const FString& JsonStr,
			const FString& bimId
			);
	};

	FEnergyInfo EnergyInfo;

	struct FEnergyValue
	{
		FString message;

		struct FbimModelDeviceAssociations
		{
			FString deviceId;

			float Value = 0.f;
		};

		/**
		 * BimID, 数据.仅一条
		 */
		TArray<FbimModelDeviceAssociations> BimIDMap;

		void Deserialize(
			const FString& JsonStr
			);

		void DoAction(
			const FString& bimId
			);
	};

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> QueryEnergyID(
		const FString& bimId
		);

	void QueryEnergyIDComplete(
		FHttpRequestPtr Req,
		FHttpResponsePtr Resp,
		bool bSuccess,
		const FString& bimId
		);

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> QueryEnergyValue(
		const FString& bimId
		);

	void QueryEnergyValueComplete(
		FHttpRequestPtr Req,
		FHttpResponsePtr Resp,
		bool bSuccess,
		const FString& bimId
		);

	struct FMQTTMSG_GetAll
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

			TMap<int32, Fchars> chars;
		};

		/**
		 * bimID，信息
		 */
		TMap<FString, Fdevices> BimID_devices;
		
		TArray<Fdevices> devices;

		struct Fchar

		{
			int32 charId;
			int32 charValue;
			FString serialid;
			FString deviceId;
		};

		/**
		 * bimID，信息
		 */
		TMap<FString, Fchar> BimID_charArray;
		
		TArray<Fchar> charArray;
	};

	UPROPERTY(Transient)
	UDTMqttClient* DTMqttClientPtr = nullptr;

	/**
	 * DeviceID,是否已订阅，是否已推送
	 */
	TMap<FString, TPair<bool, bool>> QMTTResMap;

	std::mutex mutex;

	void ConectionMQTT();

	void ConectionImp();

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

	void MessageArrivedImp_Generic(
	const FMQTTMSG_GetAll& MQTTMSG_GetAll
		);

	void MessageArrivedImp_Space(
	const FMQTTMSG_GetAll& MQTTMSG_GetAll
		);

	void UpdateMQTT();

	FTimerHandle UpdateMQTTTimerHandle;

	const int32 MaxPublishIndex = 5;

	int32 PublishIndex = 0;
};
