// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCamera.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"

// Sets default values
ABaseCamera::ABaseCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCamera::BeginPlay()
{
	Super::BeginPlay();
    // 현재 World의 GameState 가져오기
    GameState = GetWorld()->GetGameState();

}

// Called every frame
void ABaseCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UE_LOG(LogTemp, Warning, TEXT("Camera Tick"));
    
    // 인터페이스로 캐스팅 시도
    if (ICameraStateInterface* CameraState = Cast<ICameraStateInterface>(GameState))
    {
        CamLoc = CameraState->GetCameraLocation() + FVector(FMath::Min(-CamDist, -500.0f),0.0f ,0.0f);
        CamRot = CameraState->GetCameraRotation();
        CamDist = CameraState->GetCameraDistance();

        UE_LOG(LogTemp, Warning, TEXT("CamLoc: %s"), *CamLoc.ToString());
        UE_LOG(LogTemp, Warning, TEXT("CamDist: %f"), CamDist);

        // 예: 거리 기반으로 줌 설정 등도 가능

    }
    
}

