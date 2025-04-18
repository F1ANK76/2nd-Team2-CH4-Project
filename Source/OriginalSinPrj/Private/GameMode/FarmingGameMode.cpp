// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FarmingGameMode.h"
#include "GameState/FarmingGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"
#include "KillZone.h"
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
    SpawnCamera();
    SpawnKillZone();

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

    SpawnedCharacters[0]->OnChangedState.AddDynamic(this, &AFarmingGameMode::OnCharacterStateReceived);

    if (IsValid(SpawnedCharacters[0]))
    {
        AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    }
    RequestTurnOnPlayerUI();
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }
    //Spawn Monster
    SpawnInitialMonsters();
    SetPlayerReady();
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
    SpawnedCharacters[0]->OnChangedState.AddDynamic(this, &AFarmingGameMode::OnCharacterStateReceived);
    SpawnedCharacters[1]->OnChangedState.AddDynamic(this, &AFarmingGameMode::OnCharacterStateReceived);
  

    SpawnedCharacters[0]->SetActorRotation(FRotator::ZeroRotator);
    SpawnedCharacters[0]->SetActorLocation(PlayerSpawnLocations[0]);
    SpawnedCharacters[1]->SetActorRotation(FRotator::ZeroRotator);
    SpawnedCharacters[1]->SetActorLocation(PlayerSpawnLocations[1]);


    
    if (IsValid(SpawnedCharacters[0]))
    {
        AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    }
    if (IsValid(SpawnedCharacters[1]))
    {
        AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);
    }

    FarmingGameState->InitPlayerInfo();
    //���� ��ȣ�� ����...
    RequestTurnOnEnemyUI();
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }

    //���� ��ȯ�ϸ鼭,
    SpawnInitialMonsters();
    SetPlayerReady();

    //Turn on Timer
    if (FarmingGameState)
    {
        FarmingGameState->UpdateTimer();
        FarmingGameState->bIsFarmingStarted;
    }
    //Turn on Player Input
}


// killzone ���� �Լ�
void AFarmingGameMode::SpawnKillZone()
{
    if (!ActorKillZone) return;  // UPROPERTY�� ������ Ŭ������ ������ ����

    FVector SpawnLocation = FVector(0.f, 0.f, -500.f);  // ���ϴ� ��ġ
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AKillZone* SpawnedKillZone = GetWorld()->SpawnActor<AKillZone>(ActorKillZone, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedKillZone)
    {
        UE_LOG(LogTemp, Warning, TEXT("KillZone Spawned"));
    }
}

void AFarmingGameMode::OnCharacterStateReceived(const FCharacterStateBuffer& State)
{
    if (FarmingGameState)
    {
        FarmingGameState->UpdatePlayerInfo(State);
    }
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

        FarmingGameState->SaveTrigger++;
    }
    FarmingGameState->SetMyDataForNextLevel(FarmingGameState->Player1StateData.PlayerLevel);
}

void AFarmingGameMode::EndSingleGame()
{
    //SingleMode Farming Ended..
    // Move to next...
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            MoveLevel();
        }), 5.0f, false);

}

void AFarmingGameMode::EndMultiGame()
{
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            MoveLevel();
        }), 5.0f, false);
    
    //MultiMode Farming Ended..
    // Move to next...
}

//���� ���Ӹ�忡�� �� ���� �޾ƿ��� �Լ�.
//���� ���Ӹ�忡�� ĳ���͸� �޾ƿ��� �Լ�.


void AFarmingGameMode::MoveLevel()
{
    UWorld* World = GetWorld();
    if (IsValid(World)) 
    {
        if (NetMode == NM_Standalone)
        {
            if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
            {
                if (IsValid(MyGI))
                {
                    MyGI->RequestOpenLevelByType(ELevelType::SingleLevel, true);
                }
            }
        }
        else
        {
            if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
            {
                if (IsValid(MyGI))
                {
                    MyGI->RequestOpenLevelByType(ELevelType::MultiLevel, false);
                }
            }
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
void AFarmingGameMode::HandleMonsterKilled(AActor* DeadMonster, AActor* Killer)
{   
    CurrentMonsterCount--;
    ActiveMonsters.Remove(DeadMonster);

    // ���� 2���� ������ �� ������
    if (CurrentMonsterCount <= 2)
    {
        SpawnMissingMonsters();
    }

    if (IsValid(Killer))
    {
        (Cast<ABaseWitch>(Killer))->IncreaseExp();
    }
    
}
void AFarmingGameMode::SpawnCamera()
{
    UWorld* World = GetWorld();
    if (!IsValid(World) || !IsValid(BaseCamera)) return;

    for (const FVector& SpawnLocation : CameraSpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        ABaseCamera* SpawnedCamera = World->SpawnActor<ABaseCamera>(BaseCamera, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedCamera)
        {
            SpawnedBaseCamera.Add(SpawnedCamera);
        }
    }
}

void AFarmingGameMode::AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera)
{
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (PlayerController == nullptr) return;  // ��Ʈ�ѷ��� ������ ����

    if (PlayerController)
    {
        Camera->ActivateCamera(PlayerController);
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

        if (AWitchController* PC = Cast<AWitchController>(UGameplayStatics::GetPlayerController(this, 0)))
        {
            // ���� ĳ���� ����
            if (DefaultCharacterClass)
            {
                SpawnPlayerByCharacterType(DefaultCharacterClass, PC);
            }
            if (IsValid(PC))
            {
                PC->ResponseShowLevelWidget();
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
            AWitchController* PC = Cast<AWitchController>(*It);
            if (PC)
            {
                UE_LOG(LogTemp, Warning, TEXT("Controller Detected: %s"), *GetNameSafe(PC));

                // �������� Ŭ���̾�Ʈ�� Pawn�� Ȯ���ϰ� Possess ó��
                PC->SetIgnoreLookInput(true);
                if (PC)
                {
                    PC->SetControlRotation(FRotator::ZeroRotator);
                }
                FTimerHandle TimerHandle;
                GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, PC, index]()
                    {
                        HandleClientPossession(PC, index);
                    }), 1.0f, false);
                index++;
            }
            if (IsValid(PC))
            {
                PC->ResponseShowLevelWidget();
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
            PC->SetControlRotation(FRotator::ZeroRotator);
            PC->Possess(PawnToPossess);
            PC->SetControlRotation(FRotator::ZeroRotator);
            PawnToPossess->SetActorRotation(FRotator::ZeroRotator);
            PC->ClientRestart(PawnToPossess); // Ŭ���ʿ� ����� ���� ����
            PC->SetIgnoreLookInput(false);
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

    SetPlayerUnReady();
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

    AddCharacterOnAlivePlayers(PlayerCharacter);
    SpawnedCharacters.Add(PlayerCharacter);
    ActivePlayers.Add(PlayerCharacter);
    if (PlayerController->GetPawn())
    {
        PlayerController->UnPossess();
    }

    PlayerController->Possess(PlayerCharacter);

    if (AFarmingGameState* GS = GetGameState<AFarmingGameState>())
    {
        GS->SetPlayerPawn(PlayerCharacter);
    }
    PlayerCharacter->SetHpMode(true);
    UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(PlayerCharacter));
    UE_LOG(LogTemp, Warning, TEXT("PlayerController after possess: %s"), *GetNameSafe(PlayerController->GetPawn()));


    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            SetPlayerUnReady();
        }), 0.5f, false);

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
            AddCharacterOnAlivePlayers(SpawnedCharacter);
            if (SpawnedCharacter)
            {
                ActivePlayers.Add(SpawnedCharacter);
                SpawnedCharacters.Add(SpawnedCharacter);
                UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(SpawnedCharacter));
                SpawnedCharacter->SetHpMode(false);
            }
        }

    }
}

void AFarmingGameMode::AddCharacterOnAlivePlayers(AActor* Player)
{
    AlivePlayers.Add(Player);
}


void AFarmingGameMode::PossessCharacter(APlayerController* PC, APawn* PawnToPossess)
{
    if (!PC || !PawnToPossess) return;

    PawnToPossess->SetOwner(PC);
    PC->Possess(PawnToPossess);

    UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s by Controller: %s"), *GetNameSafe(PawnToPossess), *GetNameSafe(PC));
}

void AFarmingGameMode::SetPlayerLocation(AActor* Player)
{
    if (NetMode == ENetMode::NM_Standalone)
    {
        if (SpawnedCharacters.Num() > 0)
        {
            Player->SetActorRotation(FRotator::ZeroRotator);
            Player->SetActorLocation(PlayerReSpawnLocations[0]);
        }
    }
    else
    {
        if (SpawnedCharacters.Num() == 2)
        {
            if (Player == SpawnedCharacters[0])
            {
                Player->SetActorRotation(FRotator::ZeroRotator);
                Player->SetActorLocation(PlayerReSpawnLocations[0]);
            }
            else if (Player == SpawnedCharacters[1])
            {

                Player->SetActorRotation(FRotator::ZeroRotator);
                Player->SetActorLocation(PlayerReSpawnLocations[0]);
            }
        }
    }
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



void AFarmingGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{

}
void AFarmingGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{

}
void AFarmingGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
    if (AlivePlayers.Num() <= 0)
    {
        return;
    }
    else if (AlivePlayers.Num() == 1)
    {
        if (AlivePlayers[0] == Player)
        {
            AlivePlayers.Remove(Player);
            DeadPlayers.Add(Player);
        }
    }
    else if (AlivePlayers.Num() == 2)
    {
        if (AlivePlayers[0] == Player)
        {
            AlivePlayers.Remove(Player);
            DeadPlayers.Add(Player);
        }
        else if (AlivePlayers[1] == Player)
        {
            AlivePlayers.Remove(Player);
            DeadPlayers.Add(Player);
        }
    }
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            PlayerRespawn();
        }), 5.0f, false);
}

void AFarmingGameMode::PlayerRespawn()
{
    AActor* player = DeadPlayers[0];
    DeadPlayers.Remove(player);

    AlivePlayers.Add(player);
    
    SetPlayerLocation(player);
}

void AFarmingGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation) 
{
    HandleMonsterKilled(Monster, nullptr);
}




void AFarmingGameMode::SetPlayerUnReady()
{
    FarmingGameState->SetPlayerMove(false);
    UWorld* WorldContext = GetWorld();

    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            PC->GetPawn()->DisableInput(PC);
        }
    }
}

void AFarmingGameMode::SetPlayerUnReady(AActor* actor)
{
    FarmingGameState->SetPlayerMove(false);
    UWorld* WorldContext = GetWorld();

    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && (PC->GetPawn() == actor))
        {
            PC->GetPawn()->DisableInput(PC);
        }
    }
}

void AFarmingGameMode::SetPlayerReady()
{
    FarmingGameState->SetPlayerMove(true);

    UWorld* WorldContext = GetWorld();

    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            PC->GetPawn()->EnableInput(PC);
        }
    }
}