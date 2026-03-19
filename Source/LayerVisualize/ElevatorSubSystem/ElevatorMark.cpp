#include "ElevatorMark.h"

#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetStringLibrary.h"

AElevatorMark::AElevatorMark(
	const FObjectInitializer& ObjectInitializer
	)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ArmMeshPtr = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmMeshPtr"));
	ArmMeshPtr->SetupAttachment(RootComponent);

	BorderScenePtr = CreateDefaultSubobject<USceneComponent>(TEXT("BorderScene"));
	BorderScenePtr->SetupAttachment(ArmMeshPtr);

	BorderAddScenePtr = CreateDefaultSubobject<USceneComponent>(TEXT("BorderAddScenePtr"));
	BorderAddScenePtr->SetupAttachment(BorderScenePtr);

	FloorDecriptionText = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	FloorDecriptionText->SetupAttachment(BorderAddScenePtr);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f / 24;
}

void AElevatorMark::BeginPlay()
{
	Super::BeginPlay();

	BorderAddScenePtr->SetRelativeRotation(OffsetRot);

	SetName(ElevatorName);
	UpdateFloor(CurrentFloor);
}

void AElevatorMark::Tick(
	float DeltaTime
	)
{
	Super::Tick(DeltaTime);

	const auto Pt = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	auto Dir = (Pt - BorderScenePtr->GetComponentLocation()).GetSafeNormal();

	BorderScenePtr->SetWorldRotation(Dir.Rotation());
}

void AElevatorMark::SetName(
	const FString& Name
	)
{
	auto UIPtr = Cast<UElevatorMarkWdg>(FloorDecriptionText->GetUserWidgetObject());
	if ( UIPtr)
	{
		UIPtr->Name->SetText(FText::FromString(Name));
	}
}

void AElevatorMark::UpdateFloor(
	int32 Floor
	)
{
	CurrentFloor = Floor;
	auto UIPtr = Cast<UElevatorMarkWdg>(FloorDecriptionText->GetUserWidgetObject());
	if ( UIPtr)
	{
		UIPtr->FloorIndex->SetText(FText::FromString(UKismetStringLibrary::Conv_IntToString(CurrentFloor)));
	}
}
