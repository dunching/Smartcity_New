// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GenerateTypes.h"
#include "SceneInteractionDecorator.h"
#include "GameOptions.h"

#include "Tools.h"

class AFloorHelper;
class USceneInteractionWorldSystem;
class ASceneElement_PWR_Pipe;
class APersonMark;
class UGT_SwitchSceneElement_Tower;
class ASceneElement_DeviceBase;
class AFireMark;
class FArea_Decorator;


/**
 * 
 */
class LAYERCONTROLLER_API FAddtional_Decorator : public FDecoratorBase
{
public:
	GENERATIONCLASSINFO(
	                    FAddtional_Decorator,
	                    FDecoratorBase
	                   );

	FAddtional_Decorator();


	virtual void Entry() override;
	
	virtual void ReEntry() override;

	virtual void Quit() override;

	virtual bool IsEnd(
		) const override;

	virtual void OnOtherDecoratorEntry(
		const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
		) override;
	
	void Initial();

	/**
	 * Key:设备类型
	 * Value：参数
	 */
	TMap<FString, TMap<FString, FString>> ExtensionParamMap;

	TSet<TObjectPtr<ASceneElement_DeviceBase> >SceneElementSet;

	FGameplayTag FloorTag;
	
protected:
	virtual void OnUpdateFilterComplete(
		bool bIsOK,
		UGT_SwitchSceneElement_Base* TaskPtr
		) override;

	void Process();

private:

	bool bIsEnd = false;
};
