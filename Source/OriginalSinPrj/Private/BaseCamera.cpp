// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCamera.h"


// Sets default values
ABaseCamera::ABaseCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseCamera::BeginPlay()
{
	Super::BeginPlay();
    // 현재 World의 GameState 가져오기
    GameState = GetWorld()->GetGameState();
    if (IsValid(GameState))
    {
        CameraState = Cast<ICameraStateInterface>(GameState);
    }
    

}

// Called every frame
void ABaseCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    /*
    if (CameraState)
    {
        CamDist = CameraState->GetCameraDistance();
        CamRot = CameraState->GetCameraRotation();
        CamLoc = CameraState->GetCameraLocation() + FVector(FMath::Min(-CamDist, -500.0f) / 2.0f, 0.0f, 0.0f);

        UE_LOG(LogTemp, Warning, TEXT("CamLoc: %s"), *CamLoc.ToString());
        UE_LOG(LogTemp, Warning, TEXT("CamDist: %f"), CamDist);
    }

    CameraComponent->SetWorldLocation(FMath::VInterpTo(CameraComponent->GetComponentLocation(), CamLoc, DeltaTime, 5.0f));
    CameraComponent->SetWorldRotation(CamRot);
    */


}

void ABaseCamera::ActivateCamera(APlayerController* PC)
{
    
    if (IsValid(PC))
    {
        PC->SetViewTargetWithBlend(this, 0.0f);
    }
    
}

void ABaseCamera::UpdateCameraLocationandRotation()
{
    /*
    CamDist = CameraState->GetCameraDistance();
    CamLoc = CameraState->GetCameraLocation() + FVector(FMath::Min(-CamDist, -500.0f) / 2.5f, 0.0f, 0.0f);
    CamRot = CameraState->GetCameraRotation();
    */
    if (CameraState)
    {
        CamDist = CameraState->GetCameraDistance();
        CamRot = CameraState->GetCameraRotation();
        CamLoc = CameraState->GetCameraLocation() + FVector(FMath::Min(-CamDist, -500.0f) / 2.0f, 0.0f, 0.0f);
    }

    CameraComponent->SetWorldLocation(FMath::VInterpTo(CameraComponent->GetComponentLocation(), CamLoc, 0.01f, 5.0f));
    CameraComponent->SetWorldRotation(CamRot);

}

