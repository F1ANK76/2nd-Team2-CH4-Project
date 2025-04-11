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
    bUseSeamlessTravel = false; // Seamless Travel Ȱ��ȭ
    PlayerControllerClass = AWitchController::StaticClass();
}



void AFarmingGameMode::StartPlay()
{
    Super::StartPlay();
    //�� �̷����� ó���� �ϰ�....
}


void AFarmingGameMode::BeginPlay()
{
    Super::BeginPlay();
    //�� �̷����� ó���� �ϰ�....
}

void AFarmingGameMode::StartGame()
{

    //�غ�, ���� UI�� ����...
    
    //���� ��ȣ�� ����...
    
    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        GS->StartFarmingMode();
    }
    //���� ��ȯ�ϸ鼭,
    SpawnInitialMonsters();

    //Ÿ�̸� �۵� ��Ű��...
    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {

    }

    //�÷��̾ �̵� �����ϰ� �����...
}

void AFarmingGameMode::EndGame()
{
    //�÷��̾ �Է��� ���ϰ� ����...
    //Ÿ�̸Ӱ� �� �Ǿ �ð��� ����Ǿ��ٴ� �˸��� ����...
    //���� ����� �Ѿ �غ��ϱ�...
}


//�ʱ� 6���� ��ȯ
void AFarmingGameMode::SpawnInitialMonsters()
{
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass) return;

    for (int i = 0; i < MaxMonsterCount; ++i)
    {
        FVector SpawnLocation = FVector(
            FMath::RandRange(-1000.f, 1000.f), // X�� ����
            FMath::RandRange(-1000.f, 1000.f), // Y�� ����
            100.f                              // Z�� ����
        );
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            // �ʿ��� �ʱ�ȭ �ڵ� (��: �±� ���� ��)
        }

        CurrentMonsterCount++;
    }


    /*
    for (int i = 0; i < MaxMonsterCount; ++i)
    {
        // ����: �ʿ� ������ ��ġ���� ����
        // GetWorld()->SpawnActor<AFarmingEnemy>(SpawnLocation, SpawnRotation);
        CurrentMonsterCount++;
    }
    */
}

//6������ 2�������� �������� �� ���ڶ� �ֵ� �߰� ��ȯ�ϴ� �Լ�
void AFarmingGameMode::SpawnMissingMonsters()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned"));
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass) return;
    int32 ToSpawn = MaxMonsterCount - CurrentMonsterCount;
    for (int32 i = 0; i < ToSpawn; ++i)
    {
        // ���� ����
        FVector SpawnLocation = FVector(
            FMath::RandRange(-1000.f, 1000.f), // X�� ����
            FMath::RandRange(-1000.f, 1000.f), // Y�� ����
            100.f                              // Z�� ����
        );
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            // �ʿ��� �ʱ�ȭ �ڵ� (��: �±� ���� ��)
        }
        CurrentMonsterCount++;
    }
}


//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
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
    // ���� ��ȯ ó�� (�� �̵�, ���� �ܰ� �غ� ��)
}

//�÷��̾ ������ �α����ϸ� ����Ǵ� �Լ�.
void AFarmingGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (DefaultCharacterClass)
    {
        SpawnPlayerByCharacterType(DefaultCharacterClass, NewPlayer); // ����!
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

