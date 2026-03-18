#include "SceneElementCategory.h"

#include "DatasmithAssetUserData.h"
#include "Engine/StaticMeshActor.h"

ASceneElementCategory::ASceneElementCategory(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ASceneElementCategory::MergeOther(
	const TSoftObjectPtr<AActor>& ActorRef,
	const TPair<FName, FString>& InUserData,
	const TMap<FName, FString>& NewUserData
	)
{
	if (ActorRef)
	{
		if (ActorRef->IsA(AStaticMeshActor::StaticClass()))
		{
			auto STPtr = Cast<AStaticMeshActor>(ActorRef.LoadSynchronous());
			if (STPtr)
			{
				auto InterfacePtr = Cast<IInterface_AssetUserData>(STPtr->GetStaticMeshComponent());
				if (!InterfacePtr)
				{
					return;
				}

				auto NewComponentPtr = Cast<UStaticMeshComponent>(
																  AddComponentByClass(
																	   UStaticMeshComponent::StaticClass(),
																	   true,
																	   STPtr->GetStaticMeshComponent()->GetComponentTransform(),
																	   false
																	  )
																 );

				NewComponentPtr->SetStaticMesh(STPtr->GetStaticMeshComponent()->GetStaticMesh());

				for (int32 Index = 0; Index < STPtr->GetStaticMeshComponent()->GetNumMaterials(); Index++)
				{
					NewComponentPtr->SetMaterial(Index, STPtr->GetStaticMeshComponent()->GetMaterial(Index));
				}

				NewComponentPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				StaticMeshComponentsAry.Add(NewComponentPtr);
			}
		}
		else if (ActorRef->IsA(AActor::StaticClass()))
		{
			TArray<UStaticMeshComponent*>STCAry;
			ActorRef->GetComponents<UStaticMeshComponent>(STCAry);
			for (auto Iter : STCAry)
			{
				auto NewComponentPtr = Cast<UStaticMeshComponent>(
																  AddComponentByClass(
																	   UStaticMeshComponent::StaticClass(),
																	   true,
																	   Iter->GetComponentTransform(),
																	   false
																	  )
																 );

				NewComponentPtr->SetStaticMesh(Iter->GetStaticMesh());
				
				for (int32 Index = 0; Index < Iter->GetNumMaterials(); Index++)
				{
					NewComponentPtr->SetMaterial(Index, Iter->GetMaterial(Index));
				}

				NewComponentPtr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

				NewComponentPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				NewComponentPtr->SetRenderCustomDepth(false);

				StaticMeshComponentsAry.Add(NewComponentPtr);
			}
		}
	}
}
