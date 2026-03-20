// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Engine/World.h"

#include "GameOptions.h"
#include "MessageBody.h"

#include "MessageBody_Send.generated.h"

class FDecoratorBase;
class ASceneElement_Space;
class ASceneElement_DeviceBase;
class ASceneElement_PWR_Pipe;
class AFloorHelper;
class AViewerPawnBase;

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_SelectedFloor : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_SelectedFloor();

	TMap<ASceneElement_Space*, TSet<ASceneElement_DeviceBase*>> SpacesMap;

	TArray<ASceneElement_PWR_Pipe*> PWR_PipeAry;

	TObjectPtr<AFloorHelper> FloorHelper = nullptr;

	TMap<FString, TSoftObjectPtr<AViewerPawnBase>> PresetBuildingCameraSeat;

	/**
	 * ID
	 * 组
	 * 名字
	 */
	TArray<TTuple<FString, FString, FString>> ExtentPresetBuildingCameraSeat;
	
protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_SelectedSpace : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_SelectedSpace();

	FString SpaceName;

	struct FDeviceInfo
	{
		FString Type;

		FString DeviceID;
	};

	TArray<FDeviceInfo> DeviceIDAry;

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_SelectedDevice : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_SelectedDevice();

	TArray<FString> DeviceIDAry;

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_ViewDevice : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_ViewDevice();

	FString Type;

	FString DeviceID;

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_ClickedMonitor : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_ClickedMonitor();

	FString Type;

	FString DeviceID;

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_ViewConfigChanged : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_ViewConfigChanged();

	FViewConfig ViewConfig;

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_UE_Initialized : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_UE_Initialized();

protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

USTRUCT()
struct LAYERCONTROLLER_API FMessageBody_UE_Tick : public FMessageBody_Send
{
	GENERATED_BODY()

	FMessageBody_UE_Tick();

	FString Text;

	TSet<FString>ConnectedID;
	
protected:
	virtual TSharedPtr<FJsonObject> SerializeBody() const override;
};

