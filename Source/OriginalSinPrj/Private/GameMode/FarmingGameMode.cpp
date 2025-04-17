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
    bUseSeamlessTravel = true; // Seamless Travel Activate
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
    UE_LOG(LogTemp, Warning, TEXT("GameMode beginPlay"));
    FarmingGameState = GetGameState<AFarmingGameState>();
    
    if (NetMode == NM_Standalone)
    {
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                StartSingleGame();
            }), 5.0f, false);
    }
    else
    {
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                StartMultiGame();
            }), 5.0f, false);
    }
}


void AFarmingGameMode::StartSingleGame()
{
    //Prepare For Singleplay farming mode
    //Open UI for Single Farming 
    RequestTurnOnPlayerUI();
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }
    //Spawn Monster
    SpawnInitialMonsters();

    //Turn on timer 
    if (FarmingGameState)
    {
        FarmingGameState->UpdateTimer();

        FarmingGameState->bIsFarmingStarted;

    }

    //Turn on Player Input
}


void AFarmingGameMode::StartMultiGame()
{
    //Prepare For multiplay farming mode
    //Open UI for multi Farming 
    

    //���� ��ȣ�� ����...
    RequestTurnOnEnemyUI();
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }

    //���� ��ȯ�ϸ鼭,
    SpawnInitialMonsters();

    //Turn on Timer
    if (FarmingGameState)
    {
        FarmingGameState->UpdateTimer();
        FarmingGameState->bIsFarmingStarted;
    }
    //Turn on Player Input
}


//alerted by GameState
void AFarmingGameMode::EndGame()
{

    if (NetMode == NM_Standalone)
    {
        EndSingleGame();
    }
    else
    {
        EndMultiGame();
    }
}

void AFarmingGameMode::EndSingleGame()
{
    //SingleMode Farming Ended..
    // Move to next...
}

void AFarmingGameMode::EndMultiGame()
{
    //MultiMode Farming Ended..
    // Move to next...
}

//���� ���Ӹ�忡�� �� ���� �޾ƿ��� �Լ�.
//���� ���Ӹ�忡�� ĳ���͸� �޾ƿ��� �Լ�.


void AFarmingGameMode::MoveLevel(const FName& LevelName)
{
    UWorld* World = GetWorld();
    if (IsValid(World)) 
    {
        if (NetMode == NM_Standalone)
        {
            //������ ������ �̵��� �� �ֵ��� ����.
            //World->ServerTravel(TEXT("/Game/Maps/SingleLevel?listen"), true);
        }
        else
        {
            //������ ������ �̵��� �� �ֵ��� ����.
            //World->ServerTravel(TEXT("/Game/Maps/MatchLevel?listen"), true);
        }
    }
    
}


void AFarmingGameMode::RequestTurnOnEnemyUI()
{
    FarmingGameState->TurnOnAllUI();
    FarmingGameState->MultiPlayer++;

}


void AFarmingGameMode::RequestTurnOnPlayerUI()
{
    FarmingGameState->TurnOnPlayerUI();

}


//�ʱ� 6���� ��ȯ
void AFarmingGameMode::SpawnInitialMonsters()
{
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass || MonsterSpawnLocations.Num() == 0) return;

    for (const FVector& SpawnLocation : MonsterSpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            ActiveMonsters.Add(SpawnedMonster);
            CurrentMonsterCount++;
        }
    }
}

void AFarmingGameMode::SpawnMissingMonsters()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned"));
    
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass || MonsterSpawnLocations.Num() == 0) return;

    for (const FVector& SpawnLocation : MonsterSpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            ActiveMonsters.Add(SpawnedMonster);
            CurrentMonsterCount++;
        }
    }

}


//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void AFarmingGameMode::HandleMonsterKilled(AActor* DeadMonster, AController* Killer)
{
    CurrentMonsterCount--;
    ActiveMonsters.Remove(DeadMonster);

    // ���� 2���� ������ �� ������
    if (CurrentMonsterCount <= 2)
    {
        SpawnMissingMonsters();
    }

    // ����ġ ����
    if (FarmingGameState)
    {
        if (APlayerController* PC = Cast<APlayerController>(Killer))
        {
            FarmingGameState->AddExperienceToPlayer(PC, 20);
        }
    }
}




void AFarmingGameMode::PostSeamlessTravel()
{
    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
    Super::PostSeamlessTravel();  // Super SeamlessTravel

    NetMode = GetNetMode(); // Get Single? Multi?

    if (NetMode == NM_Standalone)
    {
        // �̱��÷��̿� �ʱ�ȭ
        UE_LOG(LogTemp, Warning, TEXT("[FarmingGameMode] SinglePlay"));

        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            // ���� ĳ���� ����
            if (DefaultCharacterClass)
            {
                SpawnPlayerByCharacterType(DefaultCharacterClass, PC);
            }
        }
    }
    else
    {
        // ��Ƽ�÷��� (���� ����)
        UE_LOG(LogTemp, Warning, TEXT("[FarmingGameMode] MultiPlay"));

        //��Ƽ�÷��� ����
        SpawnPlayers();
        int index = 0;
        // Ŭ���̾�Ʈ�� ��Ʈ�ѷ��� ĳ���͸� ��Ī��Ŵ

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = Cast<APlayerController>(*It);
            if (PC)
            {
                UE_LOG(LogTemp, Warning, TEXT("Controller Detected: %s"), *GetNameSafe(PC));
                // �������� Ŭ���̾�Ʈ�� Pawn�� Ȯ���ϰ� Possess ó��
                FTimerHandle TimerHandle;
                GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, PC, index]()
                    {
                        HandleClientPossession(PC, index);
                    }), 1.0f, false);
                index++;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Done"));
}


void AFarmingGameMode::HandleClientPossession(APlayerController* PC, int index)
{
    UE_LOG(LogTemp, Warning, TEXT("Client Possess start %s"), *GetNameSafe(PC));
    if (PC && SpawnedCharacters.Num() > 0)
    {
        APawn* PawnToPossess = SpawnedCharacters[index];

        if (PawnToPossess && IsValid(PawnToPossess))
        {
            // ���� ��Ʈ�ѷ��� ������ UnPossess ó��
            APlayerController* OldPC = PawnToPossess->GetController<APlayerController>();
            if (OldPC)
            {
                OldPC->UnPossess();  // ���� ��Ʈ�ѷ����� Pawn�� ����
            }
            // ���ο� ��Ʈ�ѷ��� �ش� Pawn�� Possess�ϵ��� ó��
            PC->Possess(PawnToPossess);
            PC->ClientRestart(PawnToPossess); // Ŭ���ʿ� ����� ���� ����


            UE_LOG(LogTemp, Warning, TEXT("Client Possessed Pawn: %s by %s"),
                *GetNameSafe(PawnToPossess), *GetNameSafe(PC));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Pawn to possess is not valid."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController is invalid or no spawned characters."));
    }
    UE_LOG(LogTemp, Warning, TEXT("Client Possess ended %s"), *GetNameSafe(PC));
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

    ABaseWitch* PlayerCharacter = GetWorld()->SpawnActor<ABaseWitch>(SpawnClass, PlayerSpawnLocations[0], FRotator::ZeroRotator, SpawnParam);
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


void AFarmingGameMode::SpawnPlayers()
{
    // ĳ���� Ÿ�� ���� (��: �⺻ ĳ���� Ŭ����)
    TArray<UClass*> CharacterClasses = { DefaultCharacterClass, DefaultCharacterClass };

    for (int32 i = 0; i < CharacterClasses.Num(); ++i)
    {
        UClass* SpawnClass = CharacterClasses[i];

        if (HasAuthority() && IsValid(SpawnClass))
        {
            // �÷��̾� ���� ���� ã��
            AActor* StartActor = FindPlayerStart(nullptr);  // Ư�� �÷��̾��� ���� ���� ã��
            if (!StartActor) continue;

            FVector StartPos = StartActor->GetActorLocation();

            FActorSpawnParameters SpawnParam;
            SpawnParam.Owner = nullptr;  // �⺻������ ����
            SpawnParam.Instigator = nullptr;  // �⺻������ ����

            // ĳ���� ����
            ABaseWitch* SpawnedCharacter = GetWorld()->SpawnActor<ABaseWitch>(SpawnClass, PlayerSpawnLocations[i], FRotator::ZeroRotator, SpawnParam);

            if (SpawnedCharacter)
            {
                SpawnedCharacters.Add(SpawnedCharacter);
                UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(SpawnedCharacter));
            }
        }
    }

    // SpawnedCharacters �迭�� ������ ĳ���͵��� Ȯ���� �� �ֽ��ϴ�.
    for (ABaseWitch* Character : SpawnedCharacters)
    {
        UE_LOG(LogTemp, Warning, TEXT("Managed Character: %s"), *GetNameSafe(Character));
    }
}



void AFarmingGameMode::PossessCharacter(APlayerController* PC, APawn* PawnToPossess)
{
    if (!PC || !PawnToPossess) return;

    PawnToPossess->SetOwner(PC);
    PC->Possess(PawnToPossess);

    UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s by Controller: %s"), *GetNameSafe(PawnToPossess), *GetNameSafe(PC));
}


void AFarmingGameMode::InitPlayerUI()
{
    //GameState�� ��� ������....
    //AFarmingState::InitPlayerUIInfo();
    
}








//Testcode...
void AFarmingGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("PostLogin Called"));
    if (DefaultCharacterClass)
    {
        SpawnPlayerByCharacterType(DefaultCharacterClass, NewPlayer); // ����!
    }

    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        //GS->ShowUI(NewPlayer);
    }
}
