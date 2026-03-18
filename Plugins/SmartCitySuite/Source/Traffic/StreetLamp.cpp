
#include "StreetLamp.h"

#include <functional>

#include <Components/SplineComponent.h>
#include <Components/InstancedStaticMeshComponent.h>
#include <Components/LocalLightComponent.h>

#include "CollisionDataStruct.h"
#include "Dynamic_SkyBase.h"
#include "WeatherSystem.h"

AStreetLamp::AStreetLamp(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	ScenceComponentPtr = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = ScenceComponentPtr;
}

AStreetLamp::AStreetLamp() :
	Super()
{
	PrimaryActorTick.bCanEverTick = false;

	ScenceComponentPtr = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = ScenceComponentPtr;
}

AStreetLamp::~AStreetLamp()
{

}

void AStreetLamp::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AStreetLamp::BeginPlay()
{
	Super::BeginPlay();

	auto WeatherSystemPtr = UWeatherSystem::GetInstance();
	EventID = WeatherSystemPtr->GetDynamicSky()->AddOnHourly(std::bind(&ThisClass::OnHoury, this, std::placeholders::_1));
}

void AStreetLamp::BeginDestroy()
{
	Super::BeginDestroy();
}

void AStreetLamp::Destroyed()
{
	auto WeatherSystemPtr = UWeatherSystem::GetInstance();
	//WeatherSystemPtr->GetDynamicSky()->RemoveOnHourly(EventID);

	Super::Destroyed();
}

void AStreetLamp::OnHoury(int32 Hour)
{
	auto CurvePtr = LightEnableStateCurve.LoadSynchronous();
	if (!CurvePtr)
	{
		return;
	}

	const auto Value = CurvePtr->GetFloatValue(Hour);

	TArray<ULocalLightComponent*>LightAry;
	GetComponents<ULocalLightComponent>(LightAry);
	for (auto Iter : LightAry)
	{
		if (Iter)
		{
			Iter->SetIntensity(Value);
		}
	}
}

AStreetLampHelper::AStreetLampHelper(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponentPtr = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponentPtr;
}

void AStreetLampHelper::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	GenerateLamps();
#endif
}

void AStreetLampHelper::GenerateLamps()
{
	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	for (auto Iter : OutActors)
	{
		auto LightPtr = Cast<AStreetLamp>(Iter);
		if (LightPtr)
		{
			LightAry.Add(LightPtr);
		}
	}

	auto TempLightAry = LightAry;
	LightAry.Empty();
	for (auto Iter : TempLightAry)
	{
		if (Iter)
		{
			Iter->Destroy();
		}
	}

	if (SplineComponentPtr)
	{
		const auto Length = SplineComponentPtr->GetSplineLength();
		for (float CurrentLength = 0; CurrentLength < Length; CurrentLength += StepDistance)
		{
			const auto SplineTransform = SplineComponentPtr->GetTransformAtDistanceAlongSpline(CurrentLength, ESplineCoordinateSpace::World);
			if (bIsTarceLand)
			{
				FCollisionObjectQueryParams ObjectQueryParams;
				ObjectQueryParams.AddObjectTypesToQuery(ECC_StreetObj);

				FHitResult OutHit;
				if (GetWorld()->LineTraceSingleByObjectType(
					OutHit,
					SplineTransform.GetLocation(),
					SplineTransform.GetLocation() + (FVector::DownVector * TraceLength),
					ObjectQueryParams
				))
				{
					FActorSpawnParameters SpawnParameters;
					SpawnParameters.Owner = this;
					auto StreetLampPtr = GetWorld()->SpawnActor<AStreetLamp>(StreetLampClass, SpawnParameters);
					if (StreetLampPtr)
					{
						StreetLampPtr->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

						StreetLampPtr->SetActorLocation(OutHit.ImpactPoint);
						StreetLampPtr->SetActorRotation(SplineTransform.GetRotation());

						LightAry.Add(StreetLampPtr);
					}
				}
			}
			else
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;
				auto StreetLampPtr = GetWorld()->SpawnActor<AStreetLamp>(StreetLampClass, SpawnParameters);
				if (StreetLampPtr)
				{
					StreetLampPtr->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

					StreetLampPtr->SetActorTransform(SplineTransform);

					LightAry.Add(StreetLampPtr);
				}
			}
		}
	}
}

void AStreetLampHelper::BeginPlay()
{
	Super::BeginPlay();

//#if !WITH_EDITOR
//	GenerateLamps();
//#endif
}

void AStreetLampHelper::Destroyed()
{
 	for (auto Iter : LightAry)
 	{
 		if (Iter)
 		{
 			Iter->Destroy();
 		}
 	}
 	LightAry.Empty();

	Super::Destroyed();
}
