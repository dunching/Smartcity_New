// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Engine/World.h"

#include "GameOptions.h"
#include "MessageBody.h"

#include "MessageBody_Receive.generated.h"

class FDecoratorBase;
class ASceneElement_Space;
class ASceneElement_DeviceBase;
class ASceneElement_PWR_Pipe;
class AFloorHelper;
class AViewerPawnBase;

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_AdjustCameraSeat : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_AdjustCameraSeat();

private:
	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	int32 MinPitch = 0;

	int32 MaxPitch = 0;

	bool bUseClampPitch = true;

	bool bAllowRotByYaw = true;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_SwitchViewArea : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_SwitchViewArea();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FGameplayTag AreaTag;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_SwitchMode : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_SwitchMode();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FGameplayTag ModeTag;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_AdjustHour : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_AdjustHour();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	int32 Hour = 0;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_AdjustWeather : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_AdjustWeather();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FGameplayTag Weather;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateViewConfig : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_UpdateViewConfig();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FViewConfig ViewConfig;

	bool bImmediatelyUpdate = true;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_LocaterDeviceByID : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_LocaterDeviceByID();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FString DeviceID;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_LocaterSpaceByID : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_LocaterSpaceByID();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FGameplayTag Floor;

	FString SpaceID;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_SwitchInteractionType : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_SwitchInteractionType();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FString InteractionType;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateRadarInfo : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_UpdateRadarInfo();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FString DeviceID;

	struct FUpdateRadarInfo
	{
		FVector2D Acceleration;

		FVector2D Position;

		FVector2D Velocity;

		FString TID;

		int32 EC = 0;

		int32 G = 0;

		int32 NumberOfTargets = 0;
	};

	TArray<FUpdateRadarInfo> Value;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_SetRelativeTransoform : public FMessageBody_Receive
{
	GENERATED_BODY()

public:
	FMessageBody_Receive_SetRelativeTransoform();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FString DeviceID;

	FTransform Transform;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateFloorDescription : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_UpdateFloorDescription();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FGameplayTag Floor;

	FString FloorDescription;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_ViewSpeacialArea : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_ViewSpeacialArea();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	TSet<FGameplayTag> FloorSet;

	TSet<FGameplayTag> PriorityHideFloorSet;

	FGameplayTag AreaTag;

	FString Seat;

	
	FString ID;

	FString Group;

	FString Name;

};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateSceneElementParam : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_UpdateSceneElementParam();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	TMap<FString, TMap<FString, FString>> ExtensionParamMap;

	bool bImmediatelyUpdate = true;

	FString PreviouJsonStr;
	
	bool bIsSame = true;

};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateSceneElementParamByArea : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_UpdateSceneElementParamByArea();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	TMap<FString, TMap<FString, FString>> ExtensionParamMap;

	bool bImmediatelyUpdate = true;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_SelectedDevices : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_SelectedDevices();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	TArray<FString> DeviceIDAry;

};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_ClearSelectedDevices : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_ClearSelectedDevices();

	virtual void DoAction() const override;

};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_Receive_UpdateQueryDeviceToken : public FMessageBody_Receive
{
	GENERATED_BODY()

	FMessageBody_Receive_UpdateQueryDeviceToken();

	virtual void Deserialize(
		const FString& JsonStr
		) override;

	virtual void DoAction() const override;

	FString QueryDeviceToken;
	
};

