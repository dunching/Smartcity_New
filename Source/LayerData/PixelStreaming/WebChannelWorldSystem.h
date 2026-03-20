// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "PixelStreamingPlayerId.h"
#include "Engine/World.h"

#include "WebChannelWorldSystem.generated.h"

class FDecoratorBase;
class IHttpRequest;
struct FMessageBody;
struct FMessageBody_Send;
struct FMessageBody_Receive;

/*
 */
UCLASS()
class LAYERDATA_API UWebChannelWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UWebChannelWorldSystem* GetInstance();

	void BindEvent();

	template <typename MSGType>
	void AddMGSStrategy();

	void SendMessage(
		const TSharedPtr<FMessageBody_Send>& Message
		);

	UFUNCTION()
	void OnInput(
		const FString& Descriptor
		);

	FString QueryDeviceToken;

	TSet<FPixelStreamingPlayerId>ConnetedPlayerIds;
	
private:
	void OnInputAsync(
		const FString& Descriptor
		);
	
	UFUNCTION()
	void OnConnectedToSignallingServerNative();

	void OnClosedConnectionNative(
		FString Str,
		FPixelStreamingPlayerId ID,
		bool bIsTrue
		);

	void NewConnectionNative(
		FString Str,
		FPixelStreamingPlayerId ID,
		bool bIsTrue
		);

	UFUNCTION()
	void OnAllConnectionsClosed(
		FString StreamerId
		);

	void MessageTickImp();

	TMap<FString, TSharedPtr<FMessageBody_Receive>> DeserializeStrategiesMap;

	FTimerHandle MessageTickTimerHandle;
};

template <typename MSGType>
void UWebChannelWorldSystem::AddMGSStrategy()
{
		auto MessageSPtr = MakeShared<MSGType>();
	DeserializeStrategiesMap.Add(MessageSPtr->GetCMDName(), MessageSPtr);
}
