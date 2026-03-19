#include "Building_ItemBase.h"

#include "Engine/StaticMeshActor.h"
#include "DatasmithAssetUserData.h"

#include "CollisionDataStruct.h"
#include "AssetRefMap.h"
#include "SmartCitySuiteTags.h"
#include "TemplateHelper.h"

ABuilding_ItemBase::ABuilding_ItemBase(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionObjectType(Wall_Object);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABuilding_ItemBase::ReplaceImp(
	AActor* ActorPtr,
	const TPair<FName, FString>& InUserData
	)
{
	Super::ReplaceImp(ActorPtr, InUserData);

	if (ActorPtr && ActorPtr->IsA(AStaticMeshActor::StaticClass()))
	{
		auto STPtr = Cast<AStaticMeshActor>(ActorPtr);
		if (STPtr)
		{
			// StaticMeshComponent->SetRelativeTransform(STPtr->GetStaticMeshComponent()->GetRelativeTransform());

			StaticMeshComponent->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());

			for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
			{
				StaticMeshComponent->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
			}
		}

		TArray<UStaticMeshComponent*> Components;
		GetComponents<UStaticMeshComponent>(Components);
		for (auto Iter : Components)
		{
			if (Iter)
			{
				Iter->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Iter->SetCollisionObjectType(Wall_Object);
				Iter->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

				Iter->SetCollisionResponseToChannel(ExternalWall_Object, ECollisionResponse::ECR_Overlap);
				Iter->SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
				Iter->SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);

				Iter->SetRenderCustomDepth(false);

				break;
			}
		}
	}
}

void ABuilding_ItemBase::Merge(
	const TSoftObjectPtr<AActor>& ActorRef,
	const TPair<FName, FString>& InUserData,
	const TMap<FName, FString>& NewUserData
	)
{
	Super::Merge(ActorRef, InUserData, NewUserData);

	SceneElementID = InUserData.Value;

	if (ActorRef.ToSoftObjectPath().IsValid())
	{
		auto STPtr = Cast<AStaticMeshActor>(ActorRef.Get());
		if (STPtr)
		{
			auto InterfacePtr = Cast<IInterface_AssetUserData>(STPtr->GetStaticMeshComponent());
			if (!InterfacePtr)
			{
				return;
			}
			auto AUDPtr = Cast<UDatasmithAssetUserData>(
			                                            InterfacePtr->GetAssetUserDataOfClass(
				                                             UDatasmithAssetUserData::StaticClass()
				                                            )
			                                           );

			auto Transform =
				STPtr->GetStaticMeshComponent()->
				       GetComponentTransform();
			
			auto NewComponentPtr = Cast<UStaticMeshComponent>(
			                                                  AddComponentByClass(
				                                                   UStaticMeshComponent::StaticClass(),
				                                                   true,
				                                                   Transform,
				                                                   false
				                                                  )
			                                                 );

			NewComponentPtr->AddAssetUserData(AUDPtr);

			NewComponentPtr->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());
			for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
			{
				NewComponentPtr->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
			}

			NewComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

			NewComponentPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewComponentPtr->SetCollisionObjectType(Device_Object);
			NewComponentPtr->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

			NewComponentPtr->SetCollisionResponseToChannel(ExternalWall_Object, ECollisionResponse::ECR_Overlap);
			NewComponentPtr->SetCollisionResponseToChannel(Floor_Object, ECollisionResponse::ECR_Overlap);
			NewComponentPtr->SetCollisionResponseToChannel(Space_Object, ECollisionResponse::ECR_Overlap);

			NewComponentPtr->SetRenderCustomDepth(false);

			StaticMeshComponentsAry.Add(NewComponentPtr);
		}

		ActorRef->Destroy();
	}
}

void ABuilding_ItemBase::SwitchInteractionType(
	const FSceneElementConditional& ConditionalSet
	)
{
	Super::SwitchInteractionType(ConditionalSet);
}

void ABuilding_ItemBase::SwitchState(
	EState State
	)
{
	switch (State)
	{
	case EState::kOriginal:
		{
			SetActorHiddenInGame(false);

			RevertOnriginalMat();
		}
		break;
	case EState::kHiden:
		{
			SetActorHiddenInGame(true);
		}
		break;
	}
}
