// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"

#include "GameOptions.h"
#include "FloorHelperBase.h"

#include "ElevatorMark.generated.h"

class UWidgetComponent;
class UTextRenderComponent;
class UStaticMeshComponent;
class AFloorHelper;
class USceneComponent;
class UTextBlock;


UCLASS()
class SMARTCITY_API UElevatorMarkWdg : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FloorIndex = nullptr;

};

UCLASS()
class SMARTCITY_API AElevatorMark :
	public AActor
{
	GENERATED_BODY()

public:
	AElevatorMark(
		const FObjectInitializer& ObjectInitializer
		);

	virtual  void BeginPlay() override;
	
	virtual void Tick(
		float DeltaTime
		) override;

	void SetName(const FString &Name);
	
	void UpdateFloor(int32 Floor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ArmMeshPtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BorderScenePtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BorderAddScenePtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> FloorDecriptionText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ElevatorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator RotSpeed = FRotator(90);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator OffsetRot = FRotator(0.000000, 90.000000, 180.000000);
	
	int32 CurrentFloor = 0;
};
