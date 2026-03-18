#include "SceneElementManaggerBase.h"

ASceneElementManaggerBase::ASceneElementManaggerBase(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ASceneElementManaggerBase::InitialSceneElement()
{
}

void ASceneElementManaggerBase::InitialEndSceneElement()
{
}

void ASceneElementManaggerBase::Merge(
	const TSoftObjectPtr<AActor>& ActorRef,
	const TPair<FName, FString>& InUserData,
	const TMap<FName, FString>& NewUserData
	)
{
	UserData.Append(NewUserData);
	if (ActorRef.ToSoftObjectPath().IsValid())
	{
		// AActor* ParentPtr = ActorRef.LoadSynchronous()->GetAttachParentActor();
		// if (ParentPtr && !GetAttachParentActor())
		// {
		// 	AttachToActor(ParentPtr, FAttachmentTransformRules::KeepWorldTransform);
		// 	SetActorTransform(ActorRef.LoadSynchronous()->GetTransform());
		// 	SetActorTransform(FTransform::Identity);
		// }

		TArray<AActor*> OutActors;
		ActorRef.LoadSynchronous()->GetAttachedActors(OutActors);
		for (auto Iter : OutActors)
		{
			Iter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ASceneElementManaggerBase::UpdateInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	CurrentConditionalSet = ConditionalSet;

	SwitchInteractionType(CurrentConditionalSet);
}

TArray<TObjectPtr<UGameplayTaskBase> > ASceneElementManaggerBase::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	TArray<TObjectPtr<UGameplayTaskBase> >Result;
	
	return Result;
}

void ASceneElementManaggerBase::UpdateExtensionParamMap(
	const TMap<FString, FString>& NewExtensionParamMap,
	bool bImmediatelyUpdate
	)
{
}

void ASceneElementManaggerBase::QuitAllState()
{
}
