// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSelectManager.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/Public/GameMode/CooperationGameMode.h"

// Sets default values
ABuffSelectManager::ABuffSelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    bReplicates = true; // 반드시 복제 설정
    SetReplicatingMovement(true); // 필요 시
}

// Called when the game starts or when spawned
void ABuffSelectManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuffSelectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABuffSelectManager::RequestBuffSelect(int32 BuffIndex)
{
    // 클라이언트에서 서버로 요청
    if (HasAuthority())
    {
        Server_RequestBuffSelect(BuffIndex);
    }
    else
    {
        Server_RequestBuffSelect(BuffIndex);
    }
}

void ABuffSelectManager::Server_RequestBuffSelect_Implementation(int32 BuffIndex)
{
    ACooperationGameMode* GM = Cast<ACooperationGameMode>(UGameplayStatics::GetGameMode(this));
    if (GM)
    {
        GM->HandleBuffSelection(this, BuffIndex);
    }
}