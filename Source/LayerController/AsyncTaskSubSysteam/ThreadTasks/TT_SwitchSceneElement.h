
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FQueued_SwitchSceneElement : public IQueuedWork
{
public:

	virtual void DoThreadedWork() override;

	virtual void Abandon() override;
};
