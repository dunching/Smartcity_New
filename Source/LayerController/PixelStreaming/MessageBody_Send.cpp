#include "MessageBody_Send.h"

#include "GameFramework/InputSettings.h"

#include "AssetRefMap.h"
#include "DatasmithSceneActor.h"
#include "FloorHelper.h"
#include "FloorHelper_Description.h"
#include "GameOptions.h"
#include "IPSSI.h"
#include "LogWriter.h"
#include "SceneElementCategory.h"
#include "SceneElement_DeviceBase.h"
#include "SceneElement_PWR_Pipe.h"
#include "SceneElement_RadarMode.h"
#include "SceneElement_Space.h"
#include "SceneInteractionDecorator.h"
#include "SceneInteractionDecorator_Addtional.h"
#include "SceneInteractionDecorator_Area.h"
#include "SceneInteractionDecorator_Mode.h"
#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"
#include "ViewTowerProcessor.h"
#include "ViewerPawnBase.h"
#include "WebChannelWorldSystem.h"

FMessageBody_SelectedFloor::FMessageBody_SelectedFloor()
{
	CMD_Name = TEXT("SelectedFloor");
}

TSharedPtr<FJsonObject> FMessageBody_SelectedFloor::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& Iter : SpacesMap)
		{
			auto SpaceValue = Iter.Key->GetSceneElementData();
			auto SpaceObj = SpaceValue->AsObject();

			TArray<TSharedPtr<FJsonValue>> DeviceArray;

			for (const auto& SecondIter : Iter.Value)
			{
				if (SecondIter)
				{
					DeviceArray.Add(SecondIter->GetSceneElementData());
				}
			}

			SpaceObj->SetArrayField(TEXT("Devices"), DeviceArray);

			Array.Add(SpaceValue);
		}

		RootJsonObj->SetArrayField(
		                           TEXT("Spaces"),
		                           Array
		                          );
	}
	
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& Iter : PresetBuildingCameraSeat)
		{
			auto PresetValueSPtr = MakeShared<FJsonValueString>(Iter.Key);

			Array.Add(PresetValueSPtr);
		}

		RootJsonObj->SetArrayField(
		                           TEXT("PresetCameraSets"),
		                           Array
		                          );
	}
	
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& Iter : ExtentPresetBuildingCameraSeat)
		{
			TSharedPtr<FJsonObject> JsonObj = MakeShareable<FJsonObject>(new FJsonObject);

			JsonObj->SetStringField(
									TEXT("ID"),
									Iter.Get<0>()
								   );

			JsonObj->SetStringField(
									TEXT("Group"),
									Iter.Get<1>()
								   );

			JsonObj->SetStringField(
									TEXT("Name"),
									Iter.Get<2>()
								   );

			auto PresetValueSPtr = MakeShared<FJsonValueObject>(JsonObj);

			Array.Add(PresetValueSPtr);
		}

		RootJsonObj->SetArrayField(
		                           TEXT("ExtentPresetCameraSets"),
		                           Array
		                          );
	}
	
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& Iter : PWR_PipeAry)
		{
			auto PresetValueSPtr = MakeShared<FJsonValueString>(Iter->GetID());

			Array.Add(PresetValueSPtr);
		}

		RootJsonObj->SetArrayField(
		                           TEXT("PipeAry"),
		                           Array
		                          );
	}
	
	if (FloorHelper)
	{
		RootJsonObj->SetStringField(
		                            TEXT("FloorTag"),
		                            FloorHelper->FloorTag.ToString()
		                           );

		RootJsonObj->SetStringField(
		                            TEXT("Floor"),
		                            FloorHelper->FloorIndexDescription
		                           );
	}

	return RootJsonObj;
}

FMessageBody_SelectedSpace::FMessageBody_SelectedSpace()
{
	CMD_Name = TEXT("SelectedSpace");
}

TSharedPtr<FJsonObject> FMessageBody_SelectedSpace::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	TArray<TSharedPtr<FJsonValue>> Array;

	for (const auto &Iter : DeviceIDAry)
	{
		TSharedPtr<FJsonObject> JsonObj = MakeShareable<FJsonObject>(new FJsonObject);

		JsonObj->SetStringField(
		                        TEXT("Type"),
		                        Iter.Type
		                       );

		JsonObj->SetStringField(
		                        TEXT("ID"),
		                        Iter.DeviceID
		                       );

		auto DeviceObjSPtr = MakeShared<FJsonValueObject>(JsonObj);

		Array.Add(DeviceObjSPtr);
	}

	RootJsonObj->SetArrayField(
	                           TEXT("DeviceIDAry"),
	                           Array
	                          );

	RootJsonObj->SetStringField(
	                            TEXT("SpaceName"),
	                            SpaceName
	                           );

	return RootJsonObj;
}

FMessageBody_SelectedDevice::FMessageBody_SelectedDevice()
{
	CMD_Name = TEXT("SelectedDevice");
}

TSharedPtr<FJsonObject> FMessageBody_SelectedDevice::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	TArray<TSharedPtr<FJsonValue>> Array;

	for (const auto &Iter : DeviceIDAry)
	{
		Array.Add(MakeShared<FJsonValueString>(Iter));
	}

	RootJsonObj->SetArrayField(
	                           TEXT("DeviceIDAry"),
	                           Array
	                          );

	return RootJsonObj;
}

FMessageBody_ViewDevice::FMessageBody_ViewDevice()
{
	CMD_Name = TEXT("ViewDevice");
}

TSharedPtr<FJsonObject> FMessageBody_ViewDevice::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	RootJsonObj->SetStringField(
	                            TEXT("Type"),
	                            Type
	                           );

	RootJsonObj->SetStringField(
	                            TEXT("SceneElementID"),
	                            DeviceID
	                           );

	return RootJsonObj;
}

FMessageBody_ClickedMonitor::FMessageBody_ClickedMonitor()
{
	CMD_Name = TEXT("ClickedMonitor");
}

TSharedPtr<FJsonObject> FMessageBody_ClickedMonitor::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	RootJsonObj->SetStringField(
								TEXT("Type"),
								Type
							   );

	RootJsonObj->SetStringField(
								TEXT("SceneElementID"),
								DeviceID
							   );

	return RootJsonObj;
}

FMessageBody_ViewConfigChanged::FMessageBody_ViewConfigChanged()
{
	CMD_Name = TEXT("ViewConfigChanged");
}

TSharedPtr<FJsonObject> FMessageBody_ViewConfigChanged::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	RootJsonObj->SetNumberField(
	                            TEXT("WallTranlucent"),
	                            ViewConfig.WallTranlucent
	                           );

	RootJsonObj->SetNumberField(
	                            TEXT("PillarTranlucent"),
	                            ViewConfig.PillarTranlucent
	                           );

	RootJsonObj->SetNumberField(
	                            TEXT("StairsTranlucent"),
	                            ViewConfig.StairsTranlucent
	                           );

	RootJsonObj->SetNumberField(
	                            TEXT("ShowCurtainWall"),
	                            ViewConfig.CurtainWallTranlucent
	                           );

	RootJsonObj->SetNumberField(
	                            TEXT("ShowFurniture"),
	                            ViewConfig.FurnitureTranlucent
	                           );

	return RootJsonObj;
}

FMessageBody_UE_Initialized::FMessageBody_UE_Initialized()
{
	CMD_Name = TEXT("UE_Initialized");
}

TSharedPtr<FJsonObject> FMessageBody_UE_Initialized::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	RootJsonObj->SetBoolField(
	                          TEXT("IsOK"),
	                          true
	                         );

	return RootJsonObj;
}

FMessageBody_UE_Tick::FMessageBody_UE_Tick()
{
	CMD_Name = TEXT("UE_Tick");
}

TSharedPtr<FJsonObject> FMessageBody_UE_Tick::SerializeBody() const
{
	TSharedPtr<FJsonObject> RootJsonObj = Super::SerializeBody();

	RootJsonObj->SetStringField(
	                            TEXT("Text"),
	                            Text
	                           );

	TArray<TSharedPtr<FJsonValue>> Array;

	return RootJsonObj;
}
