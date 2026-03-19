// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoverWidgetBase.h"

#include "Blueprint/UserWidget.h"

#include "AirQualityTips.generated.h"

class UWidgetSwitcher;
class UCanvasPanel;
class UTextBlock;
class USizeBox;
class UOverlay;

UCLASS()
class LAYERVISUALIZE_API UAirQualityTips : public UHoverWidgetReBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual FVector GetHoverPosition() override;

	void UpdateValue(float Value);
	
	FVector TargetPt;

protected:
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher = nullptr;
	
};
