#include "PixelStreamingUtils.h"

#include "MessageBody_Receive.h"
#include "WebChannelWorldSystem.h"

void UPixelStreamingUtils::InitializeDeserializeStrategies()
{
	auto Inst = UWebChannelWorldSystem::GetInstance();

	Inst->AddMGSStrategy<FMessageBody_Receive_AdjustCameraSeat>();
	Inst->AddMGSStrategy<FMessageBody_Receive_SwitchViewArea>();
}
