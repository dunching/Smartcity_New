
#include "SceneInteractionDecorator_Addtional.h"

#include "AssetRefMap.h"
#include "FloorHelper.h"
#include "MessageBody.h"
#include "SceneElementCategory.h"
#include "SceneElementManagger_AccessControl.h"
#include "SceneElement_Space.h"
#include "SceneInteractionDecorator_Area.h"
#include "SceneInteractionWorldSystem.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

FAddtional_Decorator::FAddtional_Decorator() :
                                           Super(
                                                )
{
}

void FAddtional_Decorator::Entry()
{
	FDecoratorBase::Entry();

	Initial();
}

void FAddtional_Decorator::ReEntry()
{
	FDecoratorBase::ReEntry();

	Initial();
}

void FAddtional_Decorator::Quit()
{
	bIsEnd = true;
	
	FDecoratorBase::Quit();
}

bool FAddtional_Decorator::IsEnd() const
{
	return bIsEnd;
}

void FAddtional_Decorator::OnOtherDecoratorEntry(
	const TSharedPtr<FDecoratorBase>& NewDecoratorSPtr
	)
{
	FDecoratorBase::OnOtherDecoratorEntry(NewDecoratorSPtr);

	Process();
}

void FAddtional_Decorator::Initial()
{
	SceneElementSet.Empty();
}

void FAddtional_Decorator::OnUpdateFilterComplete(
	bool bIsOK,
	UGT_SwitchSceneElement_Base* TaskPtr
	)
{
	FDecoratorBase::OnUpdateFilterComplete(bIsOK, TaskPtr);

	Process();
}

void FAddtional_Decorator::Process()
{
	FSceneElementConditional SceneActorConditional;

	TMulticastDelegate<void(
		bool,

		UGT_SwitchSceneElement_Base*


		
		)> MulticastDelegate;

	MulticastDelegate.AddLambda([this](auto,auto)
	{

		bIsEnd = true;
	});

	ON_SCOPE_EXIT
	{
		USceneInteractionWorldSystem::GetInstance()->UpdateFilter_BatchControlDevice(
			 SceneActorConditional,
			 true,
			 false,
			 MulticastDelegate,
			 SceneElementSet,
			 FloorTag
			);
	};
}
