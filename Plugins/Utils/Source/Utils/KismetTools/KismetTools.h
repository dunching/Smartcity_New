#pragma once

#include "CoreMinimal.h"
#include "CollisionQueryParams.h"
#include <Kismet/BlueprintFunctionLibrary.h>

#include "KismetTools.generated.h"

UCLASS()
class UKismetToolsHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UTILS_API static void SetCVar(
		const TCHAR* Name,
		int32 Value
		);
};
