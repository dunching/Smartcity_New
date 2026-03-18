// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GenerateTypes.generated.h"

class ADatasmithSceneActor;
class AReplaceActor;
class ASceneElementBase;
class UMaterialInterface;

UENUM()
enum class EOperatorType: uint8
{
	kLeftMouseButton,

	kRightMouseButton,

	kNone,
};


USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FStringAry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FString> StrAry;
};

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FModeDecription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Title;
};

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FSceneElementReplaceHelper
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bSkip = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bOnlyKey = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Key;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Value;

};

struct COMMONTYPE_API TFSceneElementTypeHelperKeyFuncs :
	BaseKeyFuncs<
		FSceneElementReplaceHelper, FSceneElementReplaceHelper,
		false
	>
{
private:
	typedef BaseKeyFuncs<
		FSceneElementReplaceHelper, FSceneElementReplaceHelper,
		false
	> Super;

public:
	typedef typename Super::ElementInitType ElementInitType;
	typedef typename Super::KeyInitType KeyInitType;

	static KeyInitType GetSetKey(
		ElementInitType Element
		);

	static bool Matches(
		KeyInitType A,
		KeyInitType B
		);

	static uint32 GetKeyHash(
		KeyInitType Key
		);
};

COMMONTYPE_API uint32 GetTypeHash(
	const FSceneElementReplaceHelper& SceneElementTypeHelper
	);

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FSceneElementMergeHelper
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bOnlyKey = false;

	/**
	 * 是的话 我们需要获取Actor Tag上的群组信息
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsRhinoMode = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString UniqueCode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Key;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Value;

};

COMMONTYPE_API uint32 GetTypeHash(
	const FSceneElementMergeHelper& SceneElementTypeHelper
	);

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FSceneElementManaggerMergeHelper
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString UniqueCode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FString, FStringAry> Maps;
};

COMMONTYPE_API uint32 GetTypeHash(
	const FSceneElementManaggerMergeHelper& SceneElementTypeHelper
	);

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FSceneElementManaggerKeyHelper
{
	GENERATED_BODY()

	bool operator==(const FSceneElementManaggerKeyHelper& Other) const
	{
		return FloorTag == Other.FloorTag && Group == Other.Group;
	}
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag FloorTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Group = 0;

};

COMMONTYPE_API uint32 GetTypeHash(
	const FSceneElementManaggerKeyHelper& SceneElementTypeHelper
	);

USTRUCT(BlueprintType, Blueprintable)
struct COMMONTYPE_API FSceneSpace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Key;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Value;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString SpaceNameValue;

};

COMMONTYPE_API uint32 GetTypeHash(
	const FSceneSpace& SceneElementTypeHelper
	);
