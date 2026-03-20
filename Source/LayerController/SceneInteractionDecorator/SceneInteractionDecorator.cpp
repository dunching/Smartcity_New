#include "SceneInteractionDecorator.h"

#include "AssetRefMap.h"

FDecoratorBase::FDecoratorBase(
	)
{
}

FDecoratorBase::~FDecoratorBase()
{
}

void FDecoratorBase::InitialType(
	FGameplayTag InMainDecoratorType,
	FGameplayTag InBranchDecoratorType
	)
{
	MainDecoratorType = InMainDecoratorType;
	BranchDecoratorType = InBranchDecoratorType;
}

void FDecoratorBase::Entry()
{
}

void FDecoratorBase::ReEntry()
{
}

void FDecoratorBase::Quit()
{
}

bool FDecoratorBase::IsEnd() const
{
	return false;
}

bool FDecoratorBase::NeedAsync() const
{
	return false;
}

void FDecoratorBase::OnOtherDecoratorQuit(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
}

void FDecoratorBase::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
}

bool FDecoratorBase::Operation(
	EOperatorType OperatorType
	)
{
	return false;
}

FGameplayTag FDecoratorBase::GetMainDecoratorType() const
{
	return MainDecoratorType;
}

FGameplayTag FDecoratorBase::GetBranchDecoratorType() const
{
	return BranchDecoratorType;
}

void FDecoratorBase::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
}

void FDecoratorBase::IncreaseWaitTaskCount()
{
	WaitTaskCount++;
}

void FDecoratorBase::DecreaseWaitTaskCount()
{
	WaitTaskCount--;
}

int32 FDecoratorBase::GetWaitTaskCount() const
{
	return WaitTaskCount;
}
