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
    //뭐 이런저런 처리를 하고....
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
    SpawnedCharacters[0]->SetActorLocation(PlayerSpawnLocations[0]);
    SpawnedCharacters[1]->SetActorLocation(PlayerSpawnLocations[1]);

    SpawnedCharacters[0]->SetActorRotation(FRotator::ZeroRotator);
    SpawnedCharacters[1]->SetActorRotation(FRotator::ZeroRotator);

    
    if (IsValid(SpawnedCharacters[0]))
    {
        AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    }
    if (IsValid(SpawnedCharacters[1]))
    {
        AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);
    }

    FarmingGameState->InitPlayerInfo();
    //시작 신호가 오면...
    RequestTurnOnEnemyUI();
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }

    //몬스터 소환하면서,
    SpawnInitialMonsters();

    //Turn on Timer
    if (FarmingGameState)
    {
        FarmingGameState->UpdateTimer();
        FarmingGameState->bIsFarmingStarted;
    }
    //Turn on Player Input
}


// killzone 생성 함수
void AFarmingGameMode::SpawnKillZone()
{
    if (!ActorKillZone) return;  // UPROPERTY로 설정한 클래스가 없으면 리턴

    FVector SpawnLocation = FVector(0.f, 0.f, -500.f);  // 원하는 위치
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

//이전 게임모드에서 맵 설정 받아오는 함수.
//이전 게임모드에서 캐릭터를 받아오는 함수.


void AFarmingGameMode::MoveLevel(const FName& LevelName)
{
    UWorld* World = GetWorld();
    if (IsValid(World)) 
    {
        if (NetMode == NM_Standalone)
        {
            //선택한 맵으로 이동할 수 있도록 수정.
            //World->ServerTravel(TEXT("/Game/Maps/SingleLevel?listen"), true);
        }
        else
        {
            //선택한 맵으로 이동할 수 있도록 수정.
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


//초기 6마리 소환
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


//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void AFarmingGameMode::HandleMonsterKilled(AActor* DeadMonster, AActor* Killer)
{   
    CurrentMonsterCount--;
    ActiveMonsters.Remove(DeadMonster);

    // 몬스터 2마리 이하일 때 리스폰
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
    if (PlayerController == nullptr) return;  // 컨트롤러가 없으면 종료

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
        // 싱글플레이용 초기화
        UE_LOG(LogTemp, Warning, TEXT("[FarmingGameMode] SinglePlay"));

        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            // 직접 캐릭터 스폰
            if (DefaultCharacterClass)
            {
                SpawnPlayerByCharacterType(DefaultCharacterClass, PC);
            }
        }
    }
    else
    {
        // 멀티플레이 (서버 기준)
        UE_LOG(LogTemp, Warning, TEXT("[FarmingGameMode] MultiPlay"));

        //멀티플레이 로직
        SpawnPlayers();
        int index = 0;
        // 클라이언트의 컨트롤러와 캐릭터를 매칭시킴

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            AWitchController* PC = Cast<AWitchController>(*It);
            if (PC)
            {
                UE_LOG(LogTemp, Warning, TEXT("Controller Detected: %s"), *GetNameSafe(PC));
                // 서버에서 클라이언트의 Pawn을 확인하고 Possess 처리
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
            // 기존 컨트롤러가 있으면 UnPossess 처리
            APlayerController* OldPC = PawnToPossess->GetController<APlayerController>();
            if (OldPC)
            {
                OldPC->UnPossess();  // 기존 컨트롤러에서 Pawn을 해제
            }
            // 새로운 컨트롤러가 해당 Pawn을 Possess하도록 처리
            PC->Possess(PawnToPossess);
            PC->ClientRestart(PawnToPossess); // 클라쪽에 제대로 상태 적용


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
    PlayerCharacter->SetHpMode(true);
    UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(PlayerCharacter));
    UE_LOG(LogTemp, Warning, TEXT("PlayerController after possess: %s"), *GetNameSafe(PlayerController->GetPawn()));
}


void AFarmingGameMode::SpawnPlayers()
{
    // 캐릭터 타입 정의 (예: 기본 캐릭터 클래스)
    TArray<UClass*> CharacterClasses = { DefaultCharacterClass, DefaultCharacterClass };

    for (int32 i = 0; i < CharacterClasses.Num(); ++i)
    {
        UClass* SpawnClass = CharacterClasses[i];

        if (HasAuthority() && IsValid(SpawnClass))
        {
            // 플레이어 시작 지점 찾기
            AActor* StartActor = FindPlayerStart(nullptr);  // 특정 플레이어의 시작 지점 찾기
            if (!StartActor) continue;

            FVector StartPos = StartActor->GetActorLocation();

            FActorSpawnParameters SpawnParam;
            SpawnParam.Owner = nullptr;  // 기본값으로 설정
            SpawnParam.Instigator = nullptr;  // 기본값으로 설정

            // 캐릭터 생성
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
            Player->SetActorLocation(PlayerReSpawnLocations[0]);
            Player->SetActorRotation(FRotator::ZeroRotator);
        }
    }
    else
    {
        if (SpawnedCharacters.Num() == 2)
        {
            if (Player == SpawnedCharacters[0])
            {
                Player->SetActorLocation(PlayerReSpawnLocations[0]);
                Player->SetActorRotation(FRotator::ZeroRotator);
            }
            else if (Player == SpawnedCharacters[1])
            {
                Player->SetActorLocation(PlayerReSpawnLocations[0]);
                Player->SetActorRotation(FRotator::ZeroRotator);
            }
        }
    }
}


void AFarmingGameMode::InitPlayerUI()
{
    //GameState에 명령 내리기....
    //AFarmingState::InitPlayerUIInfo();
    
}

//Testcode...
void AFarmingGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("PostLogin Called"));
    if (DefaultCharacterClass)
    {
        SpawnPlayerByCharacterType(DefaultCharacterClass, NewPlayer); // 여기!
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