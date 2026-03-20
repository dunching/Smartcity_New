#include "MessageBody.h"

#include "LogWriter.h"
#include "TemplateHelper.h"

FMessageBody::FMessageBody()
{
	Guid = FGuid::NewGuid();
}

FMessageBody::~FMessageBody()
{
}

TSharedPtr<FJsonObject> FMessageBody_Send::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = MakeShareable<FJsonObject>(new FJsonObject);

	RootJsonObj->SetStringField(
	                            CMD,
	                            CMD_Name
	                           );

	RootJsonObj->SetStringField(
	                            TEXT("Guid"),
	                            Guid.ToString()
	                           );

	return RootJsonObj;
}

void FMessageBody_Send::WriteLog(const FString&CurrentJsonStr) const
{
	if (bIsWriteLog)
	{
		int32 Day = 0;
		int32 Hour = 0;
		WriteLogDate(Day, Hour);
		UKismetLogger::WriteLog(CMD_Name, CurrentJsonStr, Day, Hour);
	}
}

void FMessageBody_Send::WriteLogDate(
	int32& Day,
	int32& Hour
	) const
{
	Day = -1;
	Hour = 1;
}

void FMessageBody_Receive::Deserialize(
	const FString& JsonStr
	)
{
	CurrentJsonStr = JsonStr;

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);

	TSharedPtr<FJsonObject> jsonObject;

	FJsonSerializer::Deserialize(
	                             JsonReader,
	                             jsonObject
	                            );

	FString GuidStr;
	if (jsonObject->TryGetStringField(TEXT("Guid"), GuidStr))
	{
		Guid = FGuid(JsonStr);
	}
}

void FMessageBody_Receive::DoAction() const
{
}

void FMessageBody_Receive::WriteLog() const
{
	if (bIsWriteLog)
	{
		int32 Day = 0;
		int32 Hour = 0;
		WriteLogDate(Day, Hour);
		UKismetLogger::WriteLog(CMD_Name, CurrentJsonStr, Day, Hour);
	}
}

void FMessageBody_Receive::WriteLogDate(
	int32& Day,
	int32& Hour
	) const
{
	Day = -1;
	Hour = 1;
}

FString FMessageBody_Send::GetJsonString() const
{
	TSharedPtr<FJsonObject> RootJsonObj = SerializeBody();

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(
	                           RootJsonObj.ToSharedRef(),
	                           Writer
	                          );

	return JsonString;
}

FString FMessageBody::CMD = TEXT("CMD");

FString FMessageBody::GetCMDName() const
{
	return CMD_Name;
}
