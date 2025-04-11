// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FarmingGameMode.h"
#include "GameState/FarmingGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"
//#include "FarmingEnemy.h"


AFarmingGameMode::AFarmingGameMode()
{
    bUseSeamlessTravel = false; // Seamless Travel 활성화
    PlayerControllerClass = AWitchController::StaticClass();
}



void AFarmingGameMode::StartPlay()
{
    Super::StartPlay();
    //뭐 이런저런 처리를 하고....
}


void AFarmingGameMode::BeginPlay()
{
    Super::BeginPlay();
    //뭐 이런저런 처리를 하고....
}

void AFarmingGameMode::StartGame()
{

    //준비, 시작 UI를 띄우고...
    
    //시작 신호가 오면...
    
    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        GS->StartFarmingMode();
    }
    //몬스터 소환하면서,
    SpawnInitialMonsters();

    //타이머 작동 시키고...
    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {

    }

    //플레이어가 이동 가능하게 만들기...
}

void AFarmingGameMode::EndGame()
{
    //플레이어가 입력을 못하게 막고...
    //타이머가 다 되어서 시간이 종료되었다는 알림을 띄우고...
    //다음 라운드로 넘어갈 준비하기...
}


//초기 6마리 소환
void AFarmingGameMode::SpawnInitialMonsters()
{
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass) return;

    for (int i = 0; i < MaxMonsterCount; ++i)
    {
        FVector SpawnLocation = FVector(
            FMath::RandRange(-1000.f, 1000.f), // X축 랜덤
            FMath::RandRange(-1000.f, 1000.f), // Y축 랜덤
            100.f                              // Z는 고정
        );
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            // 필요한 초기화 코드 (예: 태그 설정 등)
        }

        CurrentMonsterCount++;
    }


    /*
    for (int i = 0; i < MaxMonsterCount; ++i)
    {
        // 예시: 맵에 지정된 위치에서 스폰
        // GetWorld()->SpawnActor<AFarmingEnemy>(SpawnLocation, SpawnRotation);
        CurrentMonsterCount++;
    }
    */
}

//6마리중 2마리까지 떨어졌을 때 모자란 애들 추가 소환하는 함수
void AFarmingGameMode::SpawnMissingMonsters()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned"));
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass) return;
    int32 ToSpawn = MaxMonsterCount - CurrentMonsterCount;
    for (int32 i = 0; i < ToSpawn; ++i)
    {
        // 스폰 로직
        FVector SpawnLocation = FVector(
            FMath::RandRange(-1000.f, 1000.f), // X축 랜덤
            FMath::RandRange(-1000.f, 1000.f), // Y축 랜덤
            100.f                              // Z는 고정
        );
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            // 필요한 초기화 코드 (예: 태그 설정 등)
        }
        CurrentMonsterCount++;
    }
}


//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void AFarmingGameMode::HandleMonsterKilled(AController* Killer)
{
    CurrentMonsterCount--;

    if (CurrentMonsterCount <= 2)
    {
        SpawnMissingMonsters();
    }

    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        if (APlayerController* PC = Cast<APlayerController>(Killer))
        {
            GS->AddExperienceToPlayer(PC, 20);
        }
    }
}


void AFarmingGameMode::HandleRoundEnded()
{
    // 라운드 전환 처리 (맵 이동, 다음 단계 준비 등)
}

//플레이어가 서버에 로그인하면 실행되는 함수.
void AFarmingGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (DefaultCharacterClass)
    {
        SpawnPlayerByCharacterType(DefaultCharacterClass, NewPlayer); // 여기!
    }

    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        GS->ShowUI(NewPlayer);
    }
}

  


void AFarmingGameMode::SpawnPlayerByCharacterType(UClass* SpawnClass, APlayerController* PlayerController)
{
    checkf(IsValid(SpawnClass), TEXT("Target Class is invalid"));
    if (!PlayerController) return;

    AActor* StartActor = FindPlayerStart(PlayerController);
    if (!StartActor) return;

    FVector StartPos = StartActor->GetActorLocation();

    FActorSpawnParameters SpawnParam;
    SpawnParam.Owner = PlayerController;
    SpawnParam.Instigator = PlayerController->GetPawn();

    ABaseWitch* PlayerCharacter = GetWorld()->SpawnActor<ABaseWitch>(SpawnClass, StartPos, FRotator::ZeroRotator, SpawnParam);
    if (!PlayerCharacter) return;

    if (PlayerController->GetPawn())
    {
        PlayerController->UnPossess();
    }

    PlayerController->Possess(PlayerCharacter);

    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        GS->SetPlayerPawn(PlayerCharacter);
    }

    UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(PlayerCharacter));
    UE_LOG(LogTemp, Warning, TEXT("PlayerController after possess: %s"), *GetNameSafe(PlayerController->GetPawn()));
}

