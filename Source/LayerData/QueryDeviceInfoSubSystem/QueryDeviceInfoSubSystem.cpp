#include "QueryDeviceInfoSubSystem.h"

#include "Async/Async.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

#include "TemplateHelper.h"
#include "Tools.h"
#include "DTMqttClient.h"
#include "HttpModule.h"
#include "LogWriter.h"

UQueryDeviceInfoSubSystem* UQueryDeviceInfoSubSystem::GetInstance()
{
	return Cast<UQueryDeviceInfoSubSystem>(
	                                       USubsystemBlueprintLibrary::GetWorldSubsystem(
		                                        GetWorldImp(),
		                                        UQueryDeviceInfoSubSystem::StaticClass()
		                                       )
	                                      );
}

void UQueryDeviceInfoSubSystem::StartUpdateMQTT()
{
	ConectionMQTT();
	GetWorld()->GetTimerManager().SetTimer(UpdateMQTTTimerHandle, this, &ThisClass::UpdateMQTT, 1.f, true);
}

void UQueryDeviceInfoSubSystem::StartQueryEnergyInfo()
{
}

void UQueryDeviceInfoSubSystem::StartQueryEnergyValue()
{
}

void UQueryDeviceInfoSubSystem::FQueryDeviceInfo::Deserialize(
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
				                               TEXT("bimId"),
				                               bimModelDeviceAssociations.bimId
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
				                               TEXT("serialid"),
				                               bimModelDeviceAssociations.serialid
				                              ))
				{
				}

				if (ObjSPtr->TryGetNumberField(
				                               TEXT("spaceId"),
				                               bimModelDeviceAssociations.spaceId
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
				                               TEXT("supplier"),
				                               bimModelDeviceAssociations.supplier
				                              ))
				{
				}

				if (!bimModelDeviceAssociations.bimId.IsEmpty())
				{
					BimIDMap.Add(bimModelDeviceAssociations.bimId, bimModelDeviceAssociations.deviceId);
				}
				if (!bimModelDeviceAssociations.deviceId.IsEmpty())
				{
					if (auto GroupIter = DeviceMap.Find(bimModelDeviceAssociations.deviceId))
					{
						GroupIter->Add(bimModelDeviceAssociations.bimId, bimModelDeviceAssociations);
					}
					else
					{
						DeviceMap.Add(
						              bimModelDeviceAssociations.deviceId,
						              {{bimModelDeviceAssociations.bimId, bimModelDeviceAssociations}}
						             );
					}
				}
			}
		}
	}
}

void UQueryDeviceInfoSubSystem::FEnergyInfo::Deserialize(
	const FString& JsonStr,
	const FString& bimId
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
		const TArray<TSharedPtr<FJsonValue>>* bimModelDeviceAssociations_OutArray = nullptr;
		if ((*body_OutObject)->TryGetArrayField(
		                                        TEXT("bimModelDeviceAssociations"),
		                                        bimModelDeviceAssociations_OutArray
		                                       ))
		{
#if LOGQUERY

			if (bimModelDeviceAssociations_OutArray->IsEmpty())
			{
				{
					int32 Day = -1;
					int32 Hour = -1;
					UKismetLogger::WriteLog(TEXT("EnergyBimIDNoInfo"), bimId, Day, Hour);
				}
			}
#endif

			for (auto Iter : *bimModelDeviceAssociations_OutArray)
			{
				auto bimModelDeviceAssociations = MakeShared<FbimModelDeviceAssociations>();
				auto ObjSPtr = Iter->AsObject();

				if (ObjSPtr->TryGetNumberField(
				                               TEXT("id"),
				                               bimModelDeviceAssociations->id
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("status"),
				                               bimModelDeviceAssociations->status
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("bimId"),
				                               bimModelDeviceAssociations->bimId
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("deviceId"),
				                               bimModelDeviceAssociations->deviceId
				                              ))
				{
				}

				if (ObjSPtr->TryGetStringField(
				                               TEXT("serialid"),
				                               bimModelDeviceAssociations->serialid
				                              ))
				{
				}

				BimIDMap.Add(bimModelDeviceAssociations->bimId, bimModelDeviceAssociations);
			}
		}
	}
}

TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> UQueryDeviceInfoSubSystem::QuerySpaceInfo()
{
	// 1. URL
	FString QueryDeviceAddress = TEXT(
	                                  "192.168.88.150:18080/do"
	                                 );

	auto Path = FPaths::ProjectContentDir() /
	            TEXT("Configs") /
	            TEXT("RuntimeConfig.ini");
	Path =
		FConfigCacheIni::NormalizeConfigIniPath(
		                                        Path
		                                       );
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("QueryDeviceAddress"),
	                   QueryDeviceAddress,
	                   Path
	                  );
	FString Url =
		TEXT("http://") + QueryDeviceAddress;

	FString Token = TEXT(
	                     "ue_pjQ9Xv7mL2sK4nD8wR1cT6yH3bF5uN0aZqE7rM2pV8xC4kJ1tG6hB9dS3fL5yW"
	                    );
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("X-UE-Token"),
	                   Token,
	                   Path
	                  );

	// 2. data JSON（注意是字符串）
	FString DataJson =
		FString::Printf(
		                TEXT(
		                     "{\"status\":\"active\"}"
		                    )

		               );

	// 3. form-urlencoded 内容
	FString PostData = FString::Printf(
	                                   TEXT(
	                                        "service=%s&version=%s&time=%s&token=%s&accessToken=%s&salt=%s&test=%s&osc=%s&data=%s"
	                                       ),
	                                   TEXT("getSpaceInfo"),
	                                   TEXT("1.0"),
	                                   TEXT("0"),
	                                   TEXT("token"),
	                                   *Token,
	                                   TEXT("salt"),
	                                   TEXT("yes"),
	                                   TEXT("test"),
	                                   *FGenericPlatformHttp::UrlEncode(
	                                                                    DataJson
	                                                                   )
	                                   // ⚠️ 必须 URL 编码
	                                  );

	// 4. 创建请求
	auto SpaceInfoRequest =
		FHttpModule::Get().CreateRequest();

	SpaceInfoRequest->
		SetDelegateThreadPolicy(
		                        EHttpRequestDelegateThreadPolicy::CompleteOnGameThread
		                       );

	SpaceInfoRequest->SetURL(Url);
	SpaceInfoRequest->SetVerb(TEXT("POST"));
	SpaceInfoRequest->SetHeader(
	                            TEXT("Content-Type"),
	                            TEXT(
	                                 "application/x-www-form-urlencoded"
	                                )
	                           );
	SpaceInfoRequest->SetHeader(
	                            TEXT("X-UE-Token"),
	                            Token
	                           );
	SpaceInfoRequest->SetContentAsString(
	                                     PostData
	                                    );

	// 5. 回调
	SpaceInfoRequest->
		OnProcessRequestComplete().
		BindUObject(this, &ThisClass::QuerySpaceInfoComplete);

	// 6. 发送
	SpaceInfoRequest->ProcessRequest();

	return SpaceInfoRequest;
}

void UQueryDeviceInfoSubSystem::QuerySpaceInfoComplete(
	FHttpRequestPtr Req,
	FHttpResponsePtr ResponStr,
	bool bSuccess
	)
{
	if (!bSuccess || !ResponStr)
	{
		return;
	}

	QuerySpaceInfo_.Deserialize(ResponStr->GetContentAsString());

	QueryAllSpaceDeviceInfo();
}

TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> UQueryDeviceInfoSubSystem::QueryAllSpaceDeviceInfo(
	int32 Index
	)
{
	// 1. URL
	FString QueryDeviceAddress;

	auto Path = FPaths::ProjectContentDir() / TEXT("Configs") / TEXT("RuntimeConfig.ini");
	Path = FConfigCacheIni::NormalizeConfigIniPath(Path);
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("QueryDeviceAddress"),
	                   QueryDeviceAddress,
	                   Path
	                  );
	FString Url = TEXT("http://") + QueryDeviceAddress;

	FString Token;
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("X-UE-Token"),
	                   Token,
	                   Path
	                  );

	// 2. data JSON（注意是字符串）
	FString DataJson =
		FString::Printf(
		                TEXT(R"({"status":"active","pageNum":"%d", "pageSize":200})"),
		                Index
		               );

	// 3. form-urlencoded 内容
	FString PostData = FString::Printf(
	                                   TEXT(
	                                        "service=%s&version=%s&time=%s&token=%s&accessToken=%s&salt=%s&test=%s&osc=%s&data=%s"
	                                       ),
	                                   TEXT("getBimModelDeviceAssociation"),
	                                   TEXT("1.0"),
	                                   TEXT("0"),
	                                   TEXT("token"),
	                                   *Token,
	                                   TEXT("salt"),
	                                   TEXT("yes"),
	                                   TEXT("test"),
	                                   *FGenericPlatformHttp::UrlEncode(DataJson) // ⚠️ 必须 URL 编码
	                                  );

	// 4. 创建请求
	auto Request =
		FHttpModule::Get().CreateRequest();

	Request->SetDelegateThreadPolicy(EHttpRequestDelegateThreadPolicy::CompleteOnGameThread);

	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("X-UE-Token"), Token);
	Request->SetContentAsString(PostData);

	// 5. 回调
	Request->
		OnProcessRequestComplete().
		BindUObject(this, &ThisClass::QueryAllSpaceDeviceInfoComplete, Index);


	// 6. 发送
	Request->ProcessRequest();

	return Request;
}

void UQueryDeviceInfoSubSystem::QueryAllSpaceDeviceInfoComplete(
	FHttpRequestPtr Req,
	FHttpResponsePtr ResponStr,
	bool bSuccess,
	int32 Index
	)
{
	if (!bSuccess || !ResponStr)
	{
		return;
	}

	FQueryDeviceInfo DeviceInfo;

	DeviceInfo.Deserialize(ResponStr->GetContentAsString());

	if (DeviceInfo.BimIDMap.IsEmpty())
	{
	}
	else
	{
		AllDeviceInfo.BimIDMap.Append(DeviceInfo.BimIDMap);

		for (const auto& Iter : DeviceInfo.DeviceMap)
		{
			if (auto ValueIter = AllDeviceInfo.DeviceMap.Find(Iter.Key))
			{
				ValueIter->Append(Iter.Value);
			}
			else
			{
				AllDeviceInfo.DeviceMap.Add(Iter);
			}
		}

		QueryAllSpaceDeviceInfo(++Index);
	}

#if LOGQUERY

	TSharedPtr<FJsonObject> RootJsonObj = MakeShareable<FJsonObject>(new FJsonObject);


	for (const auto& Iter : AllDeviceInfo.DeviceMap)
	{
		TSharedRef<FJsonObject> JsonTracedPoses = MakeShareable(new FJsonObject);

		for (const auto& SecondIter : Iter.Value)
		{
			JsonTracedPoses->SetStringField(SecondIter.Key, SecondIter.Value);
		}

		RootJsonObj->SetObjectField(
		                            Iter.Key,
		                            JsonTracedPoses
		                           );
	}

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(
	                           RootJsonObj.ToSharedRef(),
	                           Writer
	                          );

	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("AllDeviceInfo"), JsonString, Day, Hour);
	}
#endif
}

void UQueryDeviceInfoSubSystem::FEnergyValue::Deserialize(
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

	const TArray<TSharedPtr<FJsonValue>>* Body_OutArray = nullptr;
	if (jsonObject->TryGetArrayField(
	                                 TEXT("body"),
	                                 Body_OutArray
	                                ))
	{
		for (auto Iter : *Body_OutArray)
		{
			FbimModelDeviceAssociations bimModelDeviceAssociations;

			auto ObjSPtr = Iter->AsObject();

			if (ObjSPtr->TryGetStringField(
			                               TEXT("deviceId"),
			                               bimModelDeviceAssociations.deviceId
			                              ))
			{
			}

			if (ObjSPtr->TryGetNumberField(
			                               TEXT("val"),
			                               bimModelDeviceAssociations.Value
			                              ))
			{
			}

			BimIDMap.Add(bimModelDeviceAssociations);
		}
	}
}

void UQueryDeviceInfoSubSystem::FEnergyValue::DoAction(
	const FString& bimId
	)
{
}

TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> UQueryDeviceInfoSubSystem::QueryEnergyID(
	const FString& bimId
	)
{
	// 1. URL
	FString QueryDeviceAddress;

	auto Path = FPaths::ProjectContentDir() / TEXT("Configs") / TEXT("RuntimeConfig.ini");
	Path = FConfigCacheIni::NormalizeConfigIniPath(Path);
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("QueryDeviceAddress"),
	                   QueryDeviceAddress,
	                   Path
	                  );
	FString Url = TEXT("http://") + QueryDeviceAddress;

	FString Token;
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("X-UE-Token"),
	                   Token,
	                   Path
	                  );

	// 2. data JSON（注意是字符串）
	FString DataJson =
		FString::Printf(
		                TEXT(R"({"bimId":"%s","status":"active"})"),
		                *bimId
		               );

	// 3. form-urlencoded 内容
	FString PostData = FString::Printf(
	                                   TEXT(
	                                        "service=%s&version=%s&time=%s&token=%s&accessToken=%s&salt=%s&test=%s&osc=%s&data=%s"
	                                       ),
	                                   TEXT("getBimModelDeviceAssociation"),
	                                   TEXT("1.0"),
	                                   TEXT("0"),
	                                   TEXT("token"),
	                                   *Token,
	                                   TEXT("salt"),
	                                   TEXT("yes"),
	                                   TEXT("test"),
	                                   *FGenericPlatformHttp::UrlEncode(DataJson) // ⚠️ 必须 URL 编码
	                                  );

	// 4. 创建请求
	auto Request =
		FHttpModule::Get().CreateRequest();

	Request->SetDelegateThreadPolicy(EHttpRequestDelegateThreadPolicy::CompleteOnGameThread);

	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("X-UE-Token"), Token);
	Request->SetContentAsString(PostData);

	// 5. 回调
	Request->OnProcessRequestComplete().BindLambda(
	                                               [this](
	                                               FHttpRequestPtr Req,
	                                               FHttpResponsePtr Resp,
	                                               bool bSuccess
	                                               )
	                                               {
		                                               AsyncTask(
		                                                         ENamedThreads::GameThread,
		                                                         [bSuccess, Resp, this]()
		                                                         {
			                                                         // 2️⃣ 回到主线程只做轻量更新
			                                                         if (bSuccess && Resp.IsValid())
			                                                         {
				                                                         // UE_LOG(LogTemp, Log, TEXT("HTTP Code: %d"), Resp->GetResponseCode());
				                                                         // UE_LOG(LogTemp, Log, TEXT("Response: %s"), *Resp->GetContentAsString());
			                                                         }
			                                                         else
			                                                         {
				                                                         // UE_LOG(LogTemp, Error, TEXT("Request Failed"));
			                                                         }
		                                                         }
		                                                        );
	                                               }
	                                              );
	// 6. 发送

#if LOGQUERY
	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("QueryEnergyID"), PostData, Day, Hour);
	}
#endif


	return Request;
}

void UQueryDeviceInfoSubSystem::QueryEnergyIDComplete(
	FHttpRequestPtr Req,
	FHttpResponsePtr ResponStr,
	bool bSuccess,
	const FString& bimId
	)
{
	if (!bSuccess || !ResponStr)
	{
		return;
	}

	TSharedPtr<FJsonObject> RootJsonObj = MakeShareable<FJsonObject>(new FJsonObject);

	EnergyInfo.Deserialize(ResponStr->GetContentAsString(), bimId);

	for (auto Iter : EnergyInfo.BimIDMap)
	{
		RootJsonObj->SetStringField(
		                            Iter.Key,
		                            Iter.Value->deviceId
		                           );
	}
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(
	                           RootJsonObj.ToSharedRef(),
	                           Writer
	                          );

#if LOGQUERY
	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("QueryEnergyIDComplete"), JsonString, Day, Hour);
	}
#endif
}

TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> UQueryDeviceInfoSubSystem::QueryEnergyValue(
	const FString& bimId
	)
{
	// 1. URL
	FString QueryDeviceAddress;

	auto Path = FPaths::ProjectContentDir() / TEXT("Configs") / TEXT("RuntimeConfig.ini");
	Path = FConfigCacheIni::NormalizeConfigIniPath(Path);
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("QueryDeviceAddress"),
	                   QueryDeviceAddress,
	                   Path
	                  );
	FString Url = TEXT("http://") + QueryDeviceAddress;

	FString Token;
	GConfig->GetString(
	                   TEXT("SmartCitySetting"),
	                   TEXT("X-UE-Token"),
	                   Token,
	                   Path
	                  );

	// 2. data JSON（注意是字符串）
	FString DataJson;
	if (auto Iter = EnergyInfo.BimIDMap.Find(bimId))
	{
		DataJson =
			FString::Printf(
			                TEXT(
			                     R"({  "supplier":"liangXinJiLiang","valType":"power_total","deviceId":"%s","subId":"%s","limit":1})"
			                    ),
			                *(*Iter)->deviceId,
			                *(*Iter)->serialid
			               );
	}

	// 3. form-urlencoded 内容
	FString PostData = FString::Printf(
	                                   TEXT(
	                                        "service=%s&version=%s&time=%s&token=%s&accessToken=%s&salt=%s&test=%s&osc=%s&data=%s"
	                                       ),
	                                   TEXT("deviceStatusQuery"),
	                                   TEXT("1.0"),
	                                   TEXT("0"),
	                                   TEXT("token"),
	                                   *Token,
	                                   TEXT("salt"),
	                                   TEXT("yes"),
	                                   TEXT("test"),
	                                   *FGenericPlatformHttp::UrlEncode(DataJson) // ⚠️ 必须 URL 编码
	                                  );

	// 4. 创建请求
	auto Request =
		FHttpModule::Get().CreateRequest();

	Request->SetDelegateThreadPolicy(EHttpRequestDelegateThreadPolicy::CompleteOnGameThread);

	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("X-UE-Token"), Token);
	Request->SetContentAsString(PostData);

	// 5. 回调
	Request->OnProcessRequestComplete().BindLambda(
	                                               [this](
	                                               FHttpRequestPtr Req,
	                                               FHttpResponsePtr Resp,
	                                               bool bSuccess
	                                               )
	                                               {
		                                               AsyncTask(
		                                                         ENamedThreads::GameThread,
		                                                         [bSuccess, Resp, this]()
		                                                         {
			                                                         // 2️⃣ 回到主线程只做轻量更新
			                                                         if (bSuccess && Resp.IsValid())
			                                                         {
				                                                         // UE_LOG(LogTemp, Log, TEXT("HTTP Code: %d"), Resp->GetResponseCode());
				                                                         // UE_LOG(LogTemp, Log, TEXT("Response: %s"), *Resp->GetContentAsString());
			                                                         }
			                                                         else
			                                                         {
				                                                         // UE_LOG(LogTemp, Error, TEXT("Request Failed"));
			                                                         }
		                                                         }
		                                                        );
	                                               }
	                                              );
	// 6. 发送
	// Request->ProcessRequest();

#if LOGQUERY
	if (DataJson.IsEmpty())
	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("QueryEnergyValue_NoData"), bimId + TEXT("----") + PostData, Day, Hour);
	}
	else
	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("QueryEnergyValue"), bimId + TEXT("----") + PostData, Day, Hour);
	}
#endif

	return Request;
}

void UQueryDeviceInfoSubSystem::QueryEnergyValueComplete(
	FHttpRequestPtr Req,
	FHttpResponsePtr ResponStr,
	bool bSuccess,
	const FString& bimId
	)
{
	if (!bSuccess || !ResponStr)
	{
#if LOGQUERY

		if (ResponStr)
		{
			int32 Day = -1;
			int32 Hour = -1;
			UKismetLogger::WriteLog(
			                        TEXT("QueryEnergyValueFailed"),
			                        bimId + TEXT("-----") + ResponStr->GetContentAsString(),
			                        Day,
			                        Hour
			                       );
		}
		else
		{
			int32 Day = -1;
			int32 Hour = -1;
			UKismetLogger::WriteLog(TEXT("QueryEnergyValueFailed"), bimId + TEXT("-----"), Day, Hour);
		}
#endif

		return;
	}

#if LOGQUERY

	{
		int32 Day = -1;
		int32 Hour = -1;
		UKismetLogger::WriteLog(TEXT("QueryEnergyValueComplete"), ResponStr->GetContentAsString(), Day, Hour);
	}
#endif

	FEnergyValue EnergyValue;
	EnergyValue.Deserialize(ResponStr->GetContentAsString());

#if LOGQUERY

	if (EnergyValue.BimIDMap.IsEmpty())
	{
		{
			int32 Day = -1;
			int32 Hour = -1;
			UKismetLogger::WriteLog(
			                        TEXT("EnergyValue_NoValue"),
			                        bimId + TEXT("---") + ResponStr->GetContentAsString(),
			                        Day,
			                        Hour
			                       );
		}
	}
#endif

	EnergyValue.DoAction(bimId);
}

void UQueryDeviceInfoSubSystem::FMQTTMSG_GetAll::Deserialize(
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

	if (jsonObject->TryGetStringField(TEXT("deviceId"), deviceId))
	{
	}

	const TArray<TSharedPtr<FJsonValue>>* devices_Array;
	if (jsonObject->TryGetArrayField(TEXT("devices"), devices_Array))
	{
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

						device.chars.Add(char_.charId, char_);
					}
				}
			}

			devices.Add(device);
			
			auto Inst = GetInstance();
			if (auto IDIter = Inst->AllDeviceInfo.DeviceMap.Find(deviceId))
			{
				bool bIsFound = true;
				for (auto SecondIter : *IDIter)
				{
					if (SecondIter.Value.serialid == device.serialid)
					{
						bIsFound = false;
						BimID_devices.Add(SecondIter.Key, device);
						break;
					}
				}

#if LOGQUERY

				if (bIsFound)
				{
					{
						int32 Day = -1;
						int32 Hour = -1;
						UKismetLogger::WriteLog(TEXT("FMQTTMSG_GetAll_DeviceNotfand"), JsonStr, Day, Hour);
					}
				}

#endif
			}
		}
	}

	const TArray<TSharedPtr<FJsonValue>>* charArray_Array;
	if (jsonObject->TryGetArrayField(TEXT("BimID_charArray"), charArray_Array))
	{
		BimID_charArray.Empty();
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

			if (ObjSPtr->TryGetStringField(TEXT("deviceId"), char_.deviceId))
			{
			}

			charArray.Add(char_);
			
			auto Inst = GetInstance();
			if (auto IDIter = Inst->AllDeviceInfo.DeviceMap.Find(char_.deviceId))
			{
				bool bIsFound = true;
				for (auto SecondIter : *IDIter)
				{
					if (SecondIter.Value.serialid == char_.serialid)
					{
						bIsFound = false;
						BimID_charArray.Add(SecondIter.Key, char_);
						break;
					}
				}
				if (bIsFound)
				{
#if LOGQUERY

					{
						int32 Day = -1;
						int32 Hour = -1;
						UKismetLogger::WriteLog(TEXT("FMQTTMSG_GetAll_char_Notfand"), JsonStr, Day, Hour);
					}

#endif
				}
			}
		}
	}
}

void UQueryDeviceInfoSubSystem::ConectionMQTT()
{
	if (!DTMqttClientPtr)
	{
		UDTMqttClient::CreateMqttClient(DTMqttClientPtr);

		DTMqttClientPtr->OnConnected.AddDynamic(this, &ThisClass::Connected);
		DTMqttClientPtr->OnConnectionLost.AddDynamic(this, &ThisClass::ConnectionLost);
		DTMqttClientPtr->OnMessageArrived.AddDynamic(this, &ThisClass::MessageArrived);
	}

	if (DTMqttClientPtr)
	{
		Async(
		      EAsyncExecution::ThreadPool,
		      [this]()
		      {
			      ConectionImp();
		      }
		     );
	}
}

void UQueryDeviceInfoSubSystem::ConectionImp()
{
}

void UQueryDeviceInfoSubSystem::Connected(
	const FString& Cause
	)
{
	if (Cause == TEXT("connect onSuccess called"))
	{
		PRINTFUNC();
	}
}

void UQueryDeviceInfoSubSystem::ConnectionLost(
	const FString& Cause
	)
{
	FTSTicker::GetCoreTicker().AddTicker(
	                                     FTickerDelegate::CreateLambda(
	                                                                   [this](
	                                                                   auto
	                                                                   )
	                                                                   {
		                                                                   ConectionMQTT();
		                                                                   return false;
	                                                                   }
	                                                                  ),
	                                     60.f
	                                    );

	std::unique_lock<std::mutex> lock(mutex);

	QMTTResMap.Empty();
}

void UQueryDeviceInfoSubSystem::MessageArrived(
	const FString& Topic,
	const FString& Message
	)
{
	FMQTTMSG_GetAll MQTTMSG_GetAll;
	MQTTMSG_GetAll.Deserialize(Message);

	MessageArrivedImp_Space(MQTTMSG_GetAll);
	
#if USEBACKENDDATA
	MessageArrivedImp_Generic(MQTTMSG_GetAll);
#endif
}

void UQueryDeviceInfoSubSystem::MessageArrivedImp_Generic(
	const FMQTTMSG_GetAll& MQTTMSG_GetAll
	)
{
}

void UQueryDeviceInfoSubSystem::MessageArrivedImp_Space(
	const FMQTTMSG_GetAll& MQTTMSG_GetAll
	)
{
}

void UQueryDeviceInfoSubSystem::UpdateMQTT()
{
	if (DTMqttClientPtr)
	{
		Async(
		      EAsyncExecution::ThreadPool,
		      [this]()
		      {
			      decltype(AllDeviceInfo.DeviceMap) DeviceMap;
			      {
				      std::unique_lock<std::mutex> lock(mutex);
				      DeviceMap = AllDeviceInfo.DeviceMap;
			      }
			      for (const auto& Iter : DeviceMap)
			      {
				      auto ResIter = QMTTResMap.Find(Iter.Key);
				      if (ResIter)
				      {
					      if (ResIter->Key)
					      {
						      continue;
					      }
				      }

				      bool Success;
				      FString ErrorMsg;

				      std::unique_lock<std::mutex> lock(mutex);
				      if (!DTMqttClientPtr)
				      {
					      return;
				      }

				      DTMqttClientPtr->Subscribe(
				                                 FString::Printf(
				                                                 TEXT("enno_p2_product/p2/android/%s/#"),
				                                                 *Iter.Key
				                                                ),
				                                 EDT_QualityOfService::QoS0,
				                                 Success,
				                                 ErrorMsg
				                                );
				      if (Success)
				      {
					      if (ResIter)
					      {
						      ResIter->Key = true;
					      }
					      else
					      {
						      QMTTResMap.Add(Iter.Key, {true, false});
					      }
#if LOGQUERY

					      {
						      int32 Day = -1;
						      int32 Hour = -1;
						      UKismetLogger::WriteLog(TEXT("Subscribe Suc"), Iter.Key, Day, Hour);
					      }

#endif
				      }
				      else
				      {
#if LOGQUERY

					      {
						      int32 Day = -1;
						      int32 Hour = -1;
						      UKismetLogger::WriteLog(TEXT("Subscribe Failed"), Iter.Key, Day, Hour);
					      }

#endif
					      return;
				      }
			      }
		      }
		     );

		Async(
		      EAsyncExecution::ThreadPool,
		      [this]()
		      {
			      const auto Ary = AllDeviceInfo.DeviceMap.Array();
			      for (
				      int32 Index = 0;
				      Index < MaxPublishIndex && PublishIndex < Ary.Num();
				      Index++, PublishIndex++
			      )
			      {
				      if (Ary.IsValidIndex(PublishIndex))
				      {
					      bool Success;
					      FString ErrorMsg;

					      std::unique_lock<std::mutex> lock(mutex);
					      if (!DTMqttClientPtr)
					      {
						      return;
					      }

					      auto ResIter = QMTTResMap.Find(Ary[PublishIndex].Key);
					      if (ResIter)
					      {
						      if (ResIter->Value)
						      {
							      continue;
						      }
					      }
					      else
					      {
						      continue;
					      }

					      DTMqttClientPtr->Publish(
					                               FString::Printf(
					                                               TEXT("enno_p2_product/p2/panno/%s"),
					                                               *Ary[PublishIndex].Key
					                                              ),
					                               FString::Printf(
					                                               TEXT(
					                                                    R"({"type":"GetAllDeviceInfo","time":"2025-11-04 11:06:20","id":"%s"})"
					                                                   ),
					                                               *Ary[PublishIndex].Key
					                                              ),
					                               EDT_QualityOfService::QoS0,
					                               true,
					                               Success,
					                               ErrorMsg
					                              );
					      if (Success)
					      {
						      if (ResIter)
						      {
							      ResIter->Value = true;
						      }
#if LOGQUERY

						      {
							      int32 Day = -1;
							      int32 Hour = -1;
							      UKismetLogger::WriteLog(TEXT("Publish Suc"), Ary[PublishIndex].Key, Day, Hour);
						      }

#endif
					      }
					      else
					      {
#if LOGQUERY

						      {
							      int32 Day = -1;
							      int32 Hour = -1;
							      UKismetLogger::WriteLog(TEXT("Publish Failed"), Ary[PublishIndex].Key, Day, Hour);
						      }

#endif
						      return;
					      }
				      }
			      }
			      if (PublishIndex >= Ary.Num())
			      {
				      PublishIndex = 0;
			      }
		      }
		     );
	}
	else
	{
		// ConectionMQTT();
	}
}

void UQueryDeviceInfoSubSystem::FQuerySpaceInfo::Deserialize(
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

			if (ObjSPtr->TryGetStringField(
			                               TEXT("floor"),
			                               bimModelDeviceAssociations.floor
			                              ))
			{
			}

			body_Ary.Add(bimModelDeviceAssociations.bimId, bimModelDeviceAssociations);
		}
	}
}
