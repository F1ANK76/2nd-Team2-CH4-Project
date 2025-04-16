// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameStateBase.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"
#include "BaseCamera.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABaseCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCamera();
	ICameraStateInterface* CameraState = nullptr;
	AGameStateBase* GameState = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly)
	FVector CamLoc = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FRotator CamRot = FRotator::ZeroRotator;
	
	UPROPERTY(BlueprintReadOnly)
	float CamDist = 0.0f;

	void UpdateCameraLocationandRotation();

	void ActivateCamera(APlayerController* PC);

};
