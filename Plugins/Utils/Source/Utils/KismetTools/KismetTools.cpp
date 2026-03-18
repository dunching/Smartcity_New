
#include "KismetTools.h"

void UKismetToolsHelper::SetCVar(
	const TCHAR* Name,
	int32 Value
	)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
	{
		CVar->Set(Value, ECVF_SetByCode);
	}
}
