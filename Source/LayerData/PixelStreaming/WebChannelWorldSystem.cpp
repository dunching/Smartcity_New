#include "WebChannelWorldSystem.h"

#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"
#include "PixelStreamingDelegates.h"
#include "HttpModule.h"
#include "PropertyAccess.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"

#include "Tools.h"
#include "GameplayTagsLibrary.h"
#include "PixelStreamingInputComponent.h"
#include "MessageBody.h"

UWebChannelWorldSystem* UWebChannelWorldSystem::GetInstance()
{
	return Cast<UWebChannelWorldSystem>(
	                                    USubsystemBlueprintLibrary::GetWorldSubsystem(
		                                     GetWorldImp(),
		                                     UWebChannelWorldSystem::StaticClass()
		                                    )
	                                   );
}

void UWebChannelWorldSystem::BindEvent()
{
	UPixelStreamingDelegates* Delegates = UPixelStreamingDelegates::GetPixelStreamingDelegates();

	Delegates->OnConnectedToSignallingServerNative.AddUObject(
	                                                          this,
	                                                          &ThisClass::OnConnectedToSignallingServerNative
	                                                         );

	Delegates->OnNewConnectionNative.AddUObject(
	                                            this,
	                                            &ThisClass::NewConnectionNative
	                                           );

	Delegates->OnClosedConnectionNative.AddUObject(
	                                               this,
	                                               &ThisClass::OnClosedConnectionNative
	                                              );

	Delegates->OnAllConnectionsClosed.AddDynamic(
	                                             this,
	                                             &ThisClass::OnAllConnectionsClosed
	                                            );
}

void UWebChannelWorldSystem::SendMessage(
	const TSharedPtr<FMessageBody_Send>& Message
	)
{
}

void UWebChannelWorldSystem::OnInputAsync(
	const FString& Descriptor
	)
{
	FString JsonStr = Descriptor;

	int32 Index = -1;
	if (JsonStr.FindLastChar(TEXT('}'), Index) && (Index + 1) < JsonStr.Len())
	{
		JsonStr.RemoveAt(Index + 1, JsonStr.Len() - Index + 1);
	}
	if (JsonStr.FindChar(TEXT('{'), Index) && Index < JsonStr.Len())
	{
		JsonStr.RemoveAt(0, Index);
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                             );

	if (jsonObject)
	{
		FString Name;
		if (jsonObject->TryGetStringField(FMessageBody_Receive::CMD, Name))
		{
			if (DeserializeStrategiesMap.Contains(Name))
			{
				auto DeserializeStrategy = DeserializeStrategiesMap[Name];
				
				DeserializeStrategy->Deserialize(JsonStr);
				DeserializeStrategy->DoAction();
				
				DeserializeStrategy->WriteLog();
			}
		}
	}
	else
	{
	}
}

void UWebChannelWorldSystem::OnConnectedToSignallingServerNative()
{
}

void UWebChannelWorldSystem::OnClosedConnectionNative(
	FString Str,
	FPixelStreamingPlayerId ID,
	bool bIsTrue
	)
{
	ConnetedPlayerIds.Remove(ID);
}

void UWebChannelWorldSystem::NewConnectionNative(
	FString Str,
	FPixelStreamingPlayerId ID,
	bool bIsTrue
	)
{
	ConnetedPlayerIds.Add(ID);

	if (MessageTickTimerHandle.IsValid())
	{
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
		                                       MessageTickTimerHandle,
		                                       this,
		                                       &ThisClass::MessageTickImp,
		                                       1.f,
		                                       true
		                                      );
	}
}

void UWebChannelWorldSystem::OnAllConnectionsClosed(
	FString StreamerId
	)
{
	GetWorld()->GetTimerManager().ClearTimer(MessageTickTimerHandle);

	ConnetedPlayerIds.Empty();
}

void UWebChannelWorldSystem::OnInput(
	const FString& Descriptor
	)
{
	Async(
	      EAsyncExecution::ThreadPool,
	      [Descriptor, this]()
	      {
	      	OnInputAsync(Descriptor);
	      }
	     );
}

void UWebChannelWorldSystem::MessageTickImp()
{
}
