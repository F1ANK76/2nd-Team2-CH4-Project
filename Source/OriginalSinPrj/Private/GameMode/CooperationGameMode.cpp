// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CooperationGameMode.h"
#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "Player/BuffComponent.h"

ACooperationGameMode::ACooperationGameMode()
{
    bUseSeamlessTravel = true; // Seamless Travel 활성화
    PlayerControllerClass = AWitchController::StaticClass();


    bIsStage1Cleared = false;
    bIsStage2Cleared = false;
    bIsStage3Cleared = false;
    Stage1ClearTrigger1 = false;
    Stage1ClearTrigger2 = false;
    StageIndex = 0;

    //GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());

}

void ACooperationGameMode::StartPlay()
{
    Super::StartPlay();
    //뭐 이런저런 처리를 하고....
}

void ACooperationGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GameMode beginPlay"));

    //Get GameState
    CooperationGameState = GetGameState<ACooperationGameState>();

    //Initial Stage index is 0;
    StageIndex = 0;

    SpawnCamera();

    //Open Player UI;
    //InitPlayerUI();
    InitPlayerUI();
    SpawnKillZone();
    //Game Start Condition -> Start with a timer temporarily


    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            StartGame();
        }), 5.0f, false);


}


void ACooperationGameMode::SetPlayerColorIndex()
{
    Player1ColorIndex = 0; // 혹은 ColorIndex::Red ... ColorIndex::White....
    Player2ColorIndex = 1;
}

int ACooperationGameMode::GetPlayerColorIndex(ACharacter* PlayerChar)
{
    if (!IsValid(PlayerChar)) return -1;
    AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
    PlayerChar->DisableInput(CharacterController);

    if (PlayerChar == SpawnedCharacters[0])
    {
        return Player1ColorIndex;
    }
    else if (PlayerChar == SpawnedCharacters[1])
    {
        return Player2ColorIndex;
    }

    //그 무엇도 아닌데, 캐릭터가 존재한다고 판정된 경우 -1 반환.
    return -1;
}

int ACooperationGameMode::GetPlayerColorIndex(AController* PlayController)
{
    if (!IsValid(PlayController)) return -1;

    for (int i = 0; i < SpawnedCharacters.Num(); i++)
    {
        AWitchController* CharacterController = Cast<AWitchController>(SpawnedCharacters[i]->GetController());
        if (CharacterController == PlayController)
        {
            return i;
        }
    }
    //그 무엇도 아닌데, 캐릭터가 존재한다고 판정된 경우 -1 반환.
    return -1;
}


void ACooperationGameMode::StartGame()
{
    SpawnedCharacters[0]->OnChangedState.AddDynamic(this, &ACooperationGameMode::OnCharacterStateReceived);
    SpawnedCharacters[1]->OnChangedState.AddDynamic(this, &ACooperationGameMode::OnCharacterStateReceived);
    //Camera Settings
    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);
    
    CooperationGameState->InitPlayerInfo();
    CooperationGameState->PlayerDataChanged++;
    //Functions to be processed before loading and starting the stage
    //Prepare the game, display the start UI...

    //Player Index Assignment
    SetPlayerColorIndex();

    //Set Stage index -> 1
    StageIndex = 1;
    CooperationGameState->CurrentStageIndex = StageIndex;
    //CooperationUI Turn on Stage1 UI
    //->ActiveStage1Widget();

    CooperationGameState->TurnOnStage1Widget();
    //Move Stage -> Prepare Stage 1 
    MoveNextStage();
}


void ACooperationGameMode::EndGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Game End "));

    //일단 플레이어 입력을 멈추기.
    SetPlayerUnReady();

    SpawnedCharacters[0]->SetActorLocation(PlayerResultLocations[0]);
    SpawnedCharacters[1]->SetActorLocation(PlayerResultLocations[1]);

    RequestOpenResultUI();
    /*
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            TravelLevel();
        }), 5.0f, false);


        */
    //일단 임시로 여기
    //TravelLevel();

}

void ACooperationGameMode::TravelLevel()
{
    UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
    if (MyGI)
    {
        // 원하는 함수나 변수 사용 가능
        MyGI->RequestOpenLevelByType(ELevelType::TitleLevel, false);
    }
    //게임인스턴스에 레벨 열어달라고 호출하기.
}


void ACooperationGameMode::RequestOpenResultUI()
{
    CooperationGameState->CurrentStageIndex = 4;
    CooperationGameState->TurnOnResultWidget();
}

//Stage1 준비 트리거
void ACooperationGameMode::ReadyStage1()
{
    //Prepare Stage 1

    UE_LOG(LogTemp, Warning, TEXT("Ready Stage1"));
    ResetAlivePlayers();
    //Turn off Player Input

    SetPlayerUnReady();

    //Spawn Monster
    SpawnMonsters();

    //Set to player position for stage 1
    SetPlayerLocation();


    //Game Start UI Open
    //When Game Start UI ...

    //Start Stage1
    StartStage1();
}

//Stage2 준비 트리거
void ACooperationGameMode::ReadyStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage2"));

    ResetAlivePlayers();
    SetPlayerLocation();
    //Enemy AI Spawn

    // Character Input Block
    SetPlayerUnReady();

    //Game Start UI Open까지 대기
    //When Game Start UI Ended...


    //CooperationUI Stage2로 바꾸기
    //CooperationWidget->ActiveStage2Widget();
    CooperationGameState->TurnOnStage2Widget();


    
    // 버프 완료 되었는지 확인하기
    CheckUntilAllPlayerSelectBuff();

}

//Stage3 준비 트리거
void ACooperationGameMode::ReadyStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage3"));
    ResetAlivePlayers();

    SetPlayerUnReady();

    SetPlayerLocation();
    SpawnBossMonsters();

    //보스 못 움직이게 잠시 세팅값 조정.

    // 캐릭터도 못움직이게 동결 시켜놓고.

    //Game Start UI Open까지 대기
    //Game Start UI 끝나면
    // 
    // 
    //CooperationUI Stage3로 바꾸기
    CooperationGameState->TurnOnStage3Widget();

    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage3Widget();
        }
    }



    CooperationGameState->SetStage3CameraTransform();

    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[1]);
    //Test Code
    //임시 딜레이 기능


    // 버프 완료 되었는지 확인하기
    CheckUntilAllPlayerSelectBuff();
}

//Stage1 시작 트리거
void ACooperationGameMode::StartStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage1"));

    //Turn On Player Input
    SetPlayerReady();
    CooperationGameState->bIsStage1Started = true;
    //Monster Movement...
}

//Stage2 시작 트리거
void ACooperationGameMode::StartStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage2"));
    SpawnEnemies();
    //Turn On Player Input
    SetPlayerReady();
    CooperationGameState->bIsStage2Started = true;
    //몬스터, 캐릭터 움직임 동결되어있는 것 풀기. Enemy Movement UnBlock

    //
}

//Stage3 시작 트리거
void ACooperationGameMode::StartStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage3"));

    //Turn On Player Input
    SetPlayerReady();

    StartBattle(ActivePlayers);
    if (IsValid(CooperationGameState))
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState is valid"));
        CooperationGameState->SetActorTickEnabled(true);
        CooperationGameState->TurnOnTimer();
    }

}

//Stage1 종료 트리거
void ACooperationGameMode::EndStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage1"));
    StageIndex++;
    CooperationGameState->CurrentStageIndex = StageIndex;
    CooperationGameState->bIsStage1Started = false;
    RequestTurnOnBuffSelectUI();
    MoveNextStage();
}

//Stage2 종료 트리거
void ACooperationGameMode::EndStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage2"));
    StageIndex++;
    CooperationGameState->CurrentStageIndex = StageIndex;
    CooperationGameState->bIsStage2Started = false;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}
//Stage3 종료 트리거
void ACooperationGameMode::EndStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage3"));
    CooperationGameState->TurnOffTimer();
    CooperationGameState->TurnOffStage3Widget();
    CooperationGameState->bIsStage3Started = false;
    EndGame();
}

void ACooperationGameMode::SetPlayerLocation()
{
    switch (StageIndex)
    {
    case 1:

        SpawnedCharacters[0]->SetActorLocation(PlayerSettingLocations[0]);
        SpawnedCharacters[1]->SetActorLocation(PlayerSettingLocations[1]);
        break;
    case 2:

        SpawnedCharacters[0]->SetActorLocation(PlayerSettingLocations[2]);
        SpawnedCharacters[1]->SetActorLocation(PlayerSettingLocations[3]);
        break;
    case 3:
        SpawnedCharacters[0]->SetActorLocation(PlayerSettingLocations[4]);
        SpawnedCharacters[1]->SetActorLocation(PlayerSettingLocations[5]);
        
        break;
    default:

        break;
    }
}

void ACooperationGameMode::ResetAlivePlayers()
{
    CurrentPlayerCount = 2;
    if (HasAuthority())
    {
        AlivePlayers.Empty(); // 기존 내용을 지우고
        for (AActor* Player : ActivePlayers)
        {

            if (Player) // null 체크 등 조건 넣을 수 있음
            {
                AlivePlayers.Add(Player);
            }
        }
    }
}


    //set color mode
    //set player level

void ACooperationGameMode::MoveNextStage()
{
    switch (StageIndex)
    {
    case 1:
        
        ReadyStage1();
        break;
    case 2:
        //모든 플레이어가 버프를 선택했는지 체크 되어야 밑에가 진행되어야함
        ReadyStage2();
        break;
    case 3:
        //모든 플레이어가 버프를 선택했는지 체크 되어야 밑에가 진행되어야함
        ReadyStage3();
        break;
    default:

        break;
    }
}//각 Stage 종료 후 매끄럽게 장면을 전환하기 위한 함수 필요


void ACooperationGameMode::RequestTurnOnBuffSelectUI()
{
    CooperationGameState->CreateBuffSelectUI();
}

void ACooperationGameMode::RequestTurnOffBuffSelectUI()
{
    CooperationGameState->SelectBuffPlayer++;
    CooperationGameState->CloseBuffSelectUI();
}
    
void ACooperationGameMode::ApplyBuffToBothPlayer()
{
    //버프 두개 골랐으면 스탯 적용하라고 요청.
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->ApplyBuffStat();
    }
    RequestTurnOffBuffSelectUI();
}

void ACooperationGameMode::CheckUntilAllPlayerSelectBuff()
{
    //UE_LOG(LogTemp, Warning, TEXT("Check All Player Selects Buff: %d"), CooperationGameState->bIsPlayerBuffSelect);

    if (!IsValid(CooperationGameState)) return;

    if (CooperationGameState->bIsPlayerBuffSelect >= 2)
    {
        UE_LOG(LogTemp, Log, TEXT("All Player has Selected Buffs! Move Next Stage."));
        switch (StageIndex)
        {
        case 2:
            ApplyBuffToBothPlayer();
            StartStage2();
            break;
        case 3:
            ApplyBuffToBothPlayer();
            StartStage3();
            break;
        }
    }
    else
    {
        GetWorldTimerManager().SetTimerForNextTick(this, &ACooperationGameMode::CheckUntilAllPlayerSelectBuff);
    }

}

//클리어 트리거를 가지고 있는 오브젝트의 트리거가 눌리면
void ACooperationGameMode::TriggerStage1Clear(UObject* Object)
{
    if (Object == Stage1ClearTriggerObject[0])
    {
        Stage1ClearTrigger1 = true;
    }
    else if (Object == Stage1ClearTriggerObject[1])
    {
        Stage1ClearTrigger2 = true;
    }

    if (Stage1ClearTrigger1 && Stage1ClearTrigger2)
    {
        EndStage1();
    }
}

void ACooperationGameMode::SetPlayerUnReady()
{
    CooperationGameState->SetPlayerMove(false);
    UWorld* WorldContext = GetWorld();

    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            FString ControllerName = PC->GetName();
            FString PawnName = PC->GetPawn()->GetName();

            UE_LOG(LogTemp, Warning, TEXT("Disabling input for PlayerController: %s, Pawn: %s"), *ControllerName, *PawnName);

            PC->GetPawn()->DisableInput(PC);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerController or Pawn is null (PC: %s)"), PC ? *PC->GetName() : TEXT("nullptr"));
        }
    }
}


void ACooperationGameMode::SetPlayerUnReady(AActor* actor)
{
    CooperationGameState->SetPlayerMove(false);
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

void ACooperationGameMode::SetPlayerReady()
{
    CooperationGameState->SetPlayerMove(true);
    
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

void ACooperationGameMode::SpawnEnemies()
{
    UWorld* World = GetWorld();
    if (!World || !EnemyBlueprintClass || EnemySpawnLocations.Num() == 0) return;

    for (const FVector& SpawnLocation : EnemySpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedEnemy = World->SpawnActor<AActor>(EnemyBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedEnemy)
        {
            ActiveEnemies.Add(SpawnedEnemy);
            CurrentEnemyCount++;
        }
    }
}

//초기 N마리 소환
void ACooperationGameMode::SpawnMonsters()
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

//보스 소환
void ACooperationGameMode::SpawnBossMonsters()
{
    UWorld* World = GetWorld();
    if (!World || !BossBlueprintClass || BossSpawnLocations.Num() == 0) return;

    for (const FVector& SpawnLocation : BossSpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(BossBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            ActiveBossMonster.Add(SpawnedMonster);
        }
    }
}

void ACooperationGameMode::HandlePlayerKilled(AActor* DeadPlayer, AActor* Killer)
{

    UE_LOG(LogTemp, Warning, TEXT("HandlePlayerKilled Event Begin"));
    UE_LOG(LogTemp, Warning, TEXT("ActivePlayers Size: %d"), ActivePlayers.Num());
    UE_LOG(LogTemp, Warning, TEXT("AlivePlayers Size: %d"), AlivePlayers.Num());
    if (CurrentPlayerCount > 1)
    {
        if (!IsValid(DeadPlayer))
        {
            UE_LOG(LogTemp, Warning, TEXT("DeadPlayer Is invalid"));
        }

        if (DeadPlayer == AlivePlayers[0])
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerWitch1 Die"));
            AlivePlayers.Remove(DeadPlayer);
        }
        else if (DeadPlayer == AlivePlayers[1])
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerWitch2 Die"));
            AlivePlayers.Remove(DeadPlayer);
        }
    }
    else if (CurrentPlayerCount > 0)
    {
        if (DeadPlayer == AlivePlayers[0])
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerWitch1 Die"));
            AlivePlayers.Remove(DeadPlayer);
        }
    }
    else
    {

    }
    
    CurrentPlayerCount--;



    //ActivePlayers.Remove(DeadPlayer); // 알아서 내부에서 찾고 제거함 너무 위험.
    //DeadPlayer->Destroy();   // 너무 위험
    //test Code
    if (CurrentPlayerCount <= 0)
    {
        //게임을 실패한거로 종료.
        //매치 종료
        // End Level ->   결과창 띄우기 실패 매개변수로 띄워서 처리
    }
}


void ACooperationGameMode::PlayerFallDie(AActor* DeadPlayer, AActor* Killer)
{
    ABaseWitch* Witch = Cast<ABaseWitch>(DeadPlayer);

    CooperationGameState->PlayerInfos[Witch].LifePoint--;
    //test Code
    if (IsValid(Witch))
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerWitch Die"));
        if (CooperationGameState->PlayerInfos[Witch].LifePoint < 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerWitch kill"));
            HandlePlayerKilled(DeadPlayer, Killer);
            Witch->ResetCharacterState();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));
            Respawn(Witch);
            Witch->ResetCharacterState();
        }
    }
}



void ACooperationGameMode::PlayerDie(AActor* DeadPlayer, AActor* Killer)
{    
    UE_LOG(LogTemp, Warning, TEXT("PlayerDie Event Begin"));
    ABaseWitch* Witch = Cast<ABaseWitch>(DeadPlayer);
    //test Code
    CooperationGameState->PlayerInfos[Witch].LifePoint--;

    if (IsValid(Witch))
    {
        UE_LOG(LogTemp, Warning, TEXT("Witch IsValid"));
        if (CooperationGameState->PlayerInfos[Witch].LifePoint < 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("LifePoint < 0"));
            Witch->ResetCharacterState();
            HandlePlayerKilled(DeadPlayer, Killer);
        }
        else
        {

            Witch->ResetCharacterState();
            Respawn(DeadPlayer);
        }
    }
}


void ACooperationGameMode::Respawn(AActor* DeadPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("PlayerWitch respawn"));
    DeadPlayer->SetActorLocation(RespawnLocation[0]);    
    //DeadPlayer 상태 초기화?
}


//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleMonsterKilled(AActor* DeadMonster, AActor* Killer)
{
    CurrentMonsterCount--;

    ActiveMonsters.Remove(DeadMonster); // 알아서 내부에서 찾고 제거함

    if (CurrentMonsterCount <= 0)
    {
        EndStage1();
    }
}




//적 AI가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleEnemyKilled(AActor* DeadMonster, AActor* Killer)
{
    //조건은 제대로 지정해야할듯
    CurrentEnemyCount--;
    // 죽은 몬스터를 ActiveMonsters 목록에서 제거
    ActiveEnemies.Remove(DeadMonster); // 알아서 내부에서 찾고 제거함
    
    if (CurrentEnemyCount <= 0)
    {
        EndStage2();
    }   
}

//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleBossMonsterKilled(AActor* Killer)
{
    EndStage3();
}



//낙사를 했을 때 어떻게 감지하는지. 이건 감지되고 처리하는 함수긴함.
void ACooperationGameMode::FallDie(AActor* Character)
{
    //낙사를 했을 때 어떻게 감지하는지. 이건 감지되고 처리하는 함수긴함.
    
}

void ACooperationGameMode::PostSeamlessTravel()
{
    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
    Super::PostSeamlessTravel();  // 기본 SeamlessTravel 처리

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
                }), 0.5f, false);
            index++;
        }
        if (IsValid(PC))
        {
            PC->ResponseShowLevelWidget();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Done"));
}

void ACooperationGameMode::HandleClientPossession(APlayerController* PC, int index)
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
    
    //Game Stage에서 필요한 기능을 위해 게임 스테이트에 컨트롤러 등록해두기
    CooperationGameState->RegisterInitialController(PC);
}

void ACooperationGameMode::SpawnPlayers()
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

            if (SpawnedCharacter)
            {
                SpawnedCharacters.Add(SpawnedCharacter);
                ActivePlayers.Add(SpawnedCharacter);
                CurrentPlayerCount++;

                SpawnedCharacter->SetHpMode(true);

                UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(SpawnedCharacter));
            }
        }
    }

    // SpawnedCharacters 배열로 생성된 캐릭터들을 확인할 수 있습니다.
    for (ABaseWitch* Character : SpawnedCharacters)
    {
        UE_LOG(LogTemp, Warning, TEXT("Managed Character: %s"), *GetNameSafe(Character));
    }

    AlivePlayers.Empty(); // 기존 내용을 지우고
    for (AActor* Player : ActivePlayers)
    {
        if (Player) // null 체크 등 조건 넣을 수 있음
        {
            AlivePlayers.Add(Player);
        }
    }

}

void ACooperationGameMode::SpawnKillZone()
{
    if (!ActorKillZone) return;  // UPROPERTY로 설정한 클래스가 없으면 리턴

    FVector SpawnLocation = FVector(0.f, 0.f, -500.f);  // 원하는 위치
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AKillZone* SpawnedKillZone = GetWorld()->SpawnActor<AKillZone>(ActorKillZone, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedKillZone)
    {
        UE_LOG(LogTemp, Warning, TEXT("KillZone 스폰 성공!"));
    }
}




void ACooperationGameMode::OnCharacterStateReceived(const FCharacterStateBuffer& State)
{   
    if (CooperationGameState)
    {
        CooperationGameState->UpdatePlayerInfo(State);
    }
}

void ACooperationGameMode::PossessCharacter(APlayerController* PC, APawn* PawnToPossess)
{
    if (!PC || !PawnToPossess) return;

    PawnToPossess->SetOwner(PC);
    PC->Possess(PawnToPossess);

    UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s by Controller: %s"), *GetNameSafe(PawnToPossess), *GetNameSafe(PC));


}


void ACooperationGameMode::InitPlayerUI()
{
    UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
    if (MyGI)
    {
        if (IsValid(MyGI))
        {
            MyGI->ResponseShowWidget();
        }
    }
}


void ACooperationGameMode::BossSetPlayerLocation(ACharacter* PlayerChar)
{
    if (IsValid(PlayerChar))
    {
        //Camera Settings
        AttachPlayerToCamera(PlayerChar, SpawnedBaseCamera[1]);
        // 플레이어의 정보를 비교 할까 말까...
        PlayerHijackedLocation = PlayerChar->GetActorLocation();

        PlayerChar->SetActorLocation(BossHijackingLocation[0]);
    }
}

void ACooperationGameMode::BossReturnPlayerLocation(ACharacter* PlayerChar)
{
    if (IsValid(PlayerChar))
    {
        // 플레이어의 정보를 비교 할까 말까...

        //이전 위치 저장해서 되돌리기
        PlayerChar->SetActorLocation(PlayerHijackedLocation);
        AttachPlayerToCamera(PlayerChar, SpawnedBaseCamera[0]);
    }
}


TArray<AActor*> ACooperationGameMode::StartBattle(TArray<AActor*> Players)
{
    return Players;
}




//납치하면 상대할 보스를 소환해놓기?
void ACooperationGameMode::SpawnGhostBoss()
{

}


void ACooperationGameMode::SpawnCamera()
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

void ACooperationGameMode::AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera)
{
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (PlayerController == nullptr) return;  // 컨트롤러가 없으면 종료

    if (PlayerController)
    {
        Camera->ActivateCamera(PlayerController);
    }

}

void ACooperationGameMode::HandleBuffSelection(AActor* SourceActor, int32 BuffIndex)
{
    UE_LOG(LogTemp, Log, TEXT("Received Buff Index %d from %s"), BuffIndex, *SourceActor->GetName());
    CooperationGameState->bIsPlayerBuffSelect += BuffIndex;
    // 여기서 버프 적용 로직 처리
}


void ACooperationGameMode::ApplyBuffToPlayer(APlayerController* Controller, int32 BuffIndex, EBuffType buff)
{
    // 예시: 모든 플레이어에 버프 부여
    Cast<ABaseWitch>(ActivePlayers[0])->ResponseSelectedBuff(buff);
    Cast<ABaseWitch>(ActivePlayers[1])->ResponseSelectedBuff(buff);

    CooperationGameState->SelectedBuff.Add(buff);

    CooperationGameState->bIsPlayerBuffSelect++;
}




void ACooperationGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    CooperationGameState->ApplyDamage(Attacker, Damage, HitLocation);
}

void ACooperationGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
    CooperationGameState->TakeDamage(Victim, Damage, HitLocation);
}

void ACooperationGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("OnDeathPlayer Event Begin"));
    CooperationGameState->OnDeathPlayer(Player, DeathLocation);
    PlayerDie(Player, nullptr);
}

void ACooperationGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
    CooperationGameState->OnDeathMonster(Monster, DeathLocation);
}



