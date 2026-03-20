// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Engine/World.h"

#include "GameOptions.h"

#include "MessageBody.generated.h"

class FDecoratorBase;
class ASceneElement_Space;
class ASceneElement_DeviceBase;
class ASceneElement_PWR_Pipe;
class AFloorHelper;
class AViewerPawnBase;

USTRUCT()
struct LAYERDATA_API FMessageBody
{
	GENERATED_BODY()

	FMessageBody();

	virtual ~FMessageBody();

	FString GetCMDName() const;

public:
	static FString CMD;

	FString CMD_Name;

	FGuid Guid;
};

USTRUCT()
struct LAYERDATA_API FMessageBody_Receive : public FMessageBody
{
	GENERATED_BODY()

public:
	virtual void Deserialize(
		const FString& JsonStr
		);

	virtual void DoAction() const;

	void WriteLog()const;
protected:
	bool bIsWriteLog = true;
private:
	
	virtual void WriteLogDate(int32 &Day, int32& Hour)const;

	FString CurrentJsonStr;
};


USTRUCT()
struct LAYERDATA_API FMessageBody_Send : public FMessageBody
{
	GENERATED_BODY()

public:
	FString GetJsonString() const;

	virtual TSharedPtr<FJsonObject> SerializeBody() const;

	void WriteLog(const FString&CurrentJsonStr)const;
protected:
	virtual void WriteLogDate(int32 &Day, int32& Hour)const;

	bool bIsWriteLog = true;
};

