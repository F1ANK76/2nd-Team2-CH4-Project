// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameStateBase.h"
#include "BaseCamera.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABaseCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCamera();

	AGameStateBase* GameState = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector CamLoc = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FRotator CamRot = FRotator::ZeroRotator;
	
	UPROPERTY(BlueprintReadOnly)
	float CamDist = 0.0f;


};
