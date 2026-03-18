#include "InputProcessor.h"

#include "XmlFile.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "InputProcessorSubSystemBase.h"
#include "Kismet/KismetStringLibrary.h"
#include "KismetTools.h"

FInputProcessor::FInputProcessor(
	FOwnerPawnType* CharacterPtr
	) :
	  OnwerPawnPtr(CharacterPtr)
{
}

FInputProcessor::~FInputProcessor()
{
}

void FInputProcessor::TickImp(
	float Delta
	)
{
}

void FInputProcessor::SwitchShowCursor(
	bool bIsShowCursor
	)
{
	UInputProcessorSubSystemBase::GetInstanceBase()->SwitchShowCursor(bIsShowCursor);
}

void FInputProcessor::SwitchRender(
	bool bIsOpen
	)
{
	auto XmlPath = FPaths::ProjectContentDir() / TEXT("Configs/Config.xml");
	XmlPath = FConfigCacheIni::NormalizeConfigIniPath(XmlPath);

	FXmlFile XmlFile;

	if (!XmlFile.LoadFile(XmlPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load XML file"));
		return;
	}

	// 获取根节点
	FXmlNode* RootNode = XmlFile.GetRootNode();
	if (!RootNode)
	{
		UE_LOG(LogTemp, Error, TEXT("No Root Node"));
		return;
	}

	FXmlNode* WeaponNode = RootNode->FindChildNode(TEXT("Render"));
	if (WeaponNode)
	{
		auto Ary = WeaponNode->GetChildrenNodes();
		for (auto Iter : Ary)
		{
			auto Tag = Iter->GetTag();
			
			if (bIsOpen)
			{
				FXmlNode* OpenNode = Iter->FindChildNode(TEXT("Open"));

				if (OpenNode)
				{
					auto Value = UKismetStringLibrary::Conv_StringToInt( OpenNode->GetContent());
					
					UKismetToolsHelper::SetCVar(*Tag, Value);
				}
			}
			else
			{
				FXmlNode* CloseNode = Iter->FindChildNode(TEXT("Close"));

				if (CloseNode)
				{
					auto Value = UKismetStringLibrary::Conv_StringToInt( CloseNode->GetContent());
					
					UKismetToolsHelper::SetCVar(*Tag, Value);
				}
			}
		}
	}
}

bool FInputProcessor::Tick(
	float Delta
	)
{
	IncreaseAsyncTaskNum();

	TickImp(Delta);

	ReduceAsyncTaskNum();

#if WITH_EDITOR
	return !bIsRequestQuit;
#else
	return !bIsRequestQuit;
#endif
}

void FInputProcessor::EnterAction()
{
	bIsRequestQuit = false;

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(
	                                                          FTickerDelegate::CreateRaw(this, &ThisClass::Tick),
	                                                          Frequency
	                                                         );
}

void FInputProcessor::ReEnterAction()
{
}

void FInputProcessor::QuitAction()
{
	if (OnQuitFunc)
	{
		OnQuitFunc();
	}

	UnRegisterTicker();
}

void FInputProcessor::BeginDestroy()
{
}

void FInputProcessor::GameDown()
{
	UnRegisterTicker();
}

bool FInputProcessor::InputKey(
	const FInputKeyEventArgs& EventArgs
	)
{
	return true;
}

bool FInputProcessor::InputAxis(
	const FInputKeyEventArgs& EventArgs
	)
{
	return true;
}

bool FInputProcessor::GetIsComplete() const
{
	return AsyncTaskNum == 0;
}

void FInputProcessor::SetPawn(
	FOwnerPawnType* NewPawnPtr
	)
{
	OnwerPawnPtr = NewPawnPtr;
}

void FInputProcessor::UnRegisterTicker()
{
	bIsRequestQuit = true;

	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

void FInputProcessor::SetOnQuitFunc(
	const FOnQuitFunc& InOnQuitFunc
	)
{
	OnQuitFunc = InOnQuitFunc;
}

void FInputProcessor::IncreaseAsyncTaskNum()
{
	AsyncTaskNum++;
}

void FInputProcessor::ReduceAsyncTaskNum()
{
	if (AsyncTaskNum > 0)
	{
		AsyncTaskNum--;
	}
}

bool FInputProcessor::GetIsRequestQuit() const
{
	return bIsRequestQuit.load();
}
