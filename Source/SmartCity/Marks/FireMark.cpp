#include "FireMark.h"

#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFireMark::AFireMark(
	const FObjectInitializer& ObjectInitializer
	) :
	  Super(ObjectInitializer)
{
	RootComponent = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	NiagaraComponent = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AFireMark::Update(
	const FVector& NewLocation
	)
{
}

AWarningMark::AWarningMark(
	const FObjectInitializer& ObjectInitializer
	):
	  Super(ObjectInitializer)
{
	RootComponent = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	AnchorComponentPtr = CreateDefaultSubobject<USceneComponent>(TEXT("BorderScene"));
	AnchorComponentPtr->SetupAttachment(RootComponent);

	NiagaraComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	NiagaraComponent->SetupAttachment(AnchorComponentPtr);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f / 24;
}

void AWarningMark::Tick(
	float DeltaTime
	)
{
	Super::Tick(DeltaTime);

	const auto Pt = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	auto Dir = (Pt - AnchorComponentPtr->GetComponentLocation()).GetSafeNormal();

	AnchorComponentPtr->SetWorldRotation(Dir.Rotation());
}

void AWarningMark::Update(
	const FVector& NewLocation
	)
{
}
