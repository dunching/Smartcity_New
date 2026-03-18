#include "PlanetGameInstance.h"

#include "HttpModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/KismetStringLibrary.h"

#include "LogHelper/LogWriter.h"

#include "InputProcessorSubSystemBase.h"
#include "IPSSI.h"
#include "KismetTools.h"
#include "XmlFile.h"

void ReadXml()
{
	auto Path = FPaths::ProjectContentDir() / TEXT("Configs/Config.xml");
	Path = FConfigCacheIni::NormalizeConfigIniPath(Path);

	FXmlFile XmlFile;
	if (!XmlFile.LoadFile(Path))
	{
		UE_LOG(LogTemp, Error, TEXT("Load XML failed: %s"), *XmlFile.GetLastError());
		return;
	}

	FXmlNode* Root = XmlFile.GetRootNode();
	if (!Root) return;

	// 遍历 Building
	const TArray<FXmlNode*>& Buildings = Root->GetChildrenNodes();
	for (FXmlNode* BuildingNode : Buildings)
	{
		if (!BuildingNode || BuildingNode->GetTag() != TEXT("CMD"))
			continue;

		FString CMDStr = BuildingNode->GetAttribute(TEXT("CMDStr"));
		FString Value = BuildingNode->GetAttribute(TEXT("Value"));

		UE_LOG(LogTemp, Log, TEXT("Building: id=%s name=%s"), *CMDStr, *Value);
		
		UKismetToolsHelper::SetCVar(*CMDStr, UKismetStringLibrary::Conv_StringToInt(Value));
	}
}

void UPlanetGameInstance::Init()
{
	Super::Init();
	
	ReadXml();
}

void UPlanetGameInstance::SetTaskbarWindowName(
	const FString& NewWindowName
	)
{
	// 仅在Windows平台生效
#if PLATFORM_WINDOWS

	// FApp::SetProjectName(*NewWindowName);

	UKismetSystemLibrary::SetWindowTitle(FText::FromString(NewWindowName));

#endif
}

void UPlanetGameInstance::OnStart()
{
	Super::OnStart();

	// 之前的代码，为什么加这句记不起来了
	// GetWorldImp()->SetGameInstance(this);

	GIsExiting = false;

	// 1. 获取原始命令行
	FString CmdLine = FCommandLine::Get();
	UE_LOG(LogTemp, Log, TEXT("原始命令行：%s"), *CmdLine);

	// 2. 解析无值参数（如 -TestMode）
	TArray<FString> Tokens;
	TArray<FString> Switches;
	FCommandLine::Parse(*CmdLine, Tokens, Switches);

	// 3. 解析带值参数（如 -CustomParam=MyValue）
	if (FParse::Value(*CmdLine, TEXT("DisplayContent="), DisplayContent))
	{
		if (DisplayContent == TEXT("All"))
		{
			
		}
		else if (DisplayContent == TEXT("F1"))
		{
		}
	}
}

void UPlanetGameInstance::Shutdown()
{
	GIsExiting = true;

	FHttpModule::Get().FHttpModule::ShutdownModule();
	
	// 注意：这里提前释放的会，之后编辑器模式下保存资源会出错
	// GUObjectArray.ShutdownUObjectArray();

#if WITH_EDITOR
#endif

	UInputProcessorSubSystem_Imp::GetInstance()->ResetProcessor();

	Super::Shutdown();
	PRINTFUNC();
}

void UPlanetGameInstance::FinishDestroy()
{
	Super::FinishDestroy();
	PRINTFUNC();
}

UPlanetGameInstance::~UPlanetGameInstance()
{
	PRINTFUNC();
}
