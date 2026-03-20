// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <type_traits>

#include "InputProcessorSubSystemBase.h"

#include "HumanProcessor.h"

#include "IPSSI.generated.h"

/**
 */
UCLASS(BlueprintType, Blueprintable)
class LAYERCONTROLLER_API UInputProcessorSubSystem_Imp : public UInputProcessorSubSystemBase
{
	GENERATED_BODY()

public:
	static UInputProcessorSubSystem_Imp* GetInstance();

	/**
	 * 注意：仅可从Decorator调用 
	 * @tparam ProcessorType 
	 */
	template <typename ProcessorType>
	void SwitchToProcessor();

	/**
	 * 注意：仅可从Decorator调用 
	 * @tparam ProcessorType 
	 */
	template <typename ProcessorType>
	void SwitchToProcessor(
		const FInitSwitchFunc<ProcessorType>& InitSwitchFunc,
		const FOnQuitFunc& OnQuitFunc = nullptr
		);

private:
};

template <typename ProcessorType>
void UInputProcessorSubSystem_Imp::SwitchToProcessor()
{
	SwitchToProcessor<ProcessorType>(nullptr);
}

template <typename ProcessorType>
void UInputProcessorSubSystem_Imp::SwitchToProcessor(
	const FInitSwitchFunc<ProcessorType>& InitSwitchFunc,
	const FOnQuitFunc& OnQuitFunc
	)
{
	if constexpr (
		(std::is_same_v<ProcessorType, FInputProcessor>) 
	)
	{
		return;
	}

	if (CurrentProcessorSPtr.IsValid())
	{
		CurrentProcessorSPtr->QuitAction();
	}

	ActionCacheSet.Add(CurrentProcessorSPtr);

	TSharedPtr<ProcessorType> ActionProcessSPtr;

	if constexpr (std::is_base_of_v<HumanProcessor::FHumanProcessor, ProcessorType>)
	{
		ActionProcessSPtr = MakeShared<ProcessorType>();
		ActionProcessSPtr->SetOnQuitFunc(OnQuitFunc);
	}
	else
	{
		ActionProcessSPtr = MakeShared<ProcessorType>();
		ActionProcessSPtr->SetOnQuitFunc(OnQuitFunc);
	}

	if (InitSwitchFunc)
	{
		InitSwitchFunc(ActionProcessSPtr.Get());
	}
	CurrentProcessorSPtr = ActionProcessSPtr;

	CurrentProcessorSPtr->EnterAction();
}
