// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CooperationGameMode.h"
#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"


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
    InitPlayerUI();

    //Game Start Condition -> Start with a timer temporarily
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            StartGame();
        }), 5.0f, false);


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

    //Camera Settings

    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);

    //Functions to be processed before loading and starting the stage
    //Prepare the game, display the start UI...

    if (IsValid(CooperationGameState))
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState is valid"));
        CooperationGameState->SetActorTickEnabled(true);
    }

    //Player Index Assignment
    SetPlayerColorIndex();

    //Set Stage index -> 1
    StageIndex = 1;

    //Move Stage -> Prepare Stage 1 
    MoveNextStage();
}


void ACooperationGameMode::EndGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Game End "));

    //일단 플레이어 입력을 멈추기.
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);


    SpawnedCharacters[0]->SetActorLocation(PlayerResultLocations[0]);
    SpawnedCharacters[1]->SetActorLocation(PlayerResultLocations[1]);

    //result Page Open
    RequestOpenResultUI();


}

void ACooperationGameMode::RequestOpenResultUI()
{
    //CooperationGameState->RequestPlayerToOpenResultUI();
}

//Stage1 준비 트리거
void ACooperationGameMode::ReadyStage1()
{
    //Prepare Stage 1

    UE_LOG(LogTemp, Warning, TEXT("Ready Stage1"));

    //Turn off Player Input

    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);

    //Spawn Monster
    SpawnMonsters();

    //Set to player position for stage 1
    SetPlayerLocation();



    //CooperationUI Turn on Stage1 UI
    //->ActiveStage1Widget();

    //Game Start UI Open
    //When Game Start UI ...

    //Start Stage1
    StartStage1();
}

//Stage2 준비 트리거
void ACooperationGameMode::ReadyStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage2"));
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);
    SpawnEnemies();
    SetPlayerLocation();
    //몬스터 못움직이게 잠시 세팅값 조정.

    // 캐릭터도 못움직이게 동결 시켜놓고.

    //Game Start UI Open까지 대기
    //Game Start UI 끝나면


    //CooperationUI Stage2로 바꾸기
    //->ActiveStage2Widget();

    //몬스터, 캐릭터 움직임 동결되어있는 것 풀기.




    //Test Code
    //Delay for Test

    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &ACooperationGameMode::CheckUntilAllPlayerSelectBuff, 5.0f, false);

    
    // 버프 완료 되었는지 확인하기
    //CheckUntilAllPlayerSelectBuff();

}

//Stage3 준비 트리거
void ACooperationGameMode::ReadyStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage3"));
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);

    SetPlayerLocation();
    SpawnBossMonsters();

    //보스 못 움직이게 잠시 세팅값 조정.

    // 캐릭터도 못움직이게 동결 시켜놓고.

    //Game Start UI Open까지 대기
    //Game Start UI 끝나면
    // 
    // 
    //CooperationUI Stage3로 바꾸기
    //->ActiveStage3Widget();
    CooperationGameState->SetStage3CameraTransform();

    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[1]);
    //Test Code
    //임시 딜레이 기능

    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &ACooperationGameMode::CheckUntilAllPlayerSelectBuff, 5.0f, false);


    // 버프 완료 되었는지 확인하기
    //CheckUntilAllPlayerSelectBuff();
}

//Stage1 시작 트리거
void ACooperationGameMode::StartStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage1"));

    //Turn On Player Input
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->TurnOnTimer();
    }
    //Monster Movement...
}

//Stage2 시작 트리거
void ACooperationGameMode::StartStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage2"));

    //Turn On Player Input
    //SetPlayerReady(SpawnedCharacters[0]);
    //SetPlayerReady(SpawnedCharacters[1]);

    //Enemy Movement...
}

//Stage3 시작 트리거
void ACooperationGameMode::StartStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage3"));

    //Turn On Player Input
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);
    
    //Turn on BossMonster Behaviour Function

    if (IsValid(CooperationGameState))
    {
        //CooperationGameState->TurnOnTimer();
    }
}

//Stage1 종료 트리거
void ACooperationGameMode::EndStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage1"));
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}

//Stage2 종료 트리거
void ACooperationGameMode::EndStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage2"));
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}
//Stage3 종료 트리거
void ACooperationGameMode::EndStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage3"));

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
    /*
    //게임 스테이트 보고 좀 열어달라고 요청하기
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->RequestPlayerToOpenBuffUI();
    }
    */
}
    
void ACooperationGameMode::ApplyBuffToBothPlayer()
{
    //버프 두개 골랐으면 스탯 적용하라고 요청.
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->ApplyBuffStat();
    }
}

void ACooperationGameMode::CheckUntilAllPlayerSelectBuff()
{
    UE_LOG(LogTemp, Warning, TEXT("Check All Player Selects Buff"));

    if (!IsValid(CooperationGameState)) return;


    //testCode
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

    //testCode

    /* //테스트중... 주석 해제 해야됨
    if (CooperationGameState->bPlayer1SelectedBuff && CooperationGameState->bPlayer2SelectedBuff)
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
    */
}


/*
UFUNCTION(Client, Reliable)
void Client_DisableInput();

void AWitchController::Client_DisableInput_Implementation()
{
    DisableInput(this); // this는 Local PlayerController
}
*/

/*
AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
if (CharacterController)
{
    CharacterController->Client_DisableInput(); // 클라이언트에서 입력 비활성
}
*/



void ACooperationGameMode::SetPlayerUnReady(ACharacter* PlayerChar)
{
    if (!PlayerChar) return;
    AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
    //CharacterController->Client_DisableInput(); // 클라이언트에서 입력 비활성
}

void ACooperationGameMode::SetPlayerReady(ACharacter* PlayerChar)
{
    if (!PlayerChar) return;
    AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
    PlayerChar->EnableInput(CharacterController);
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

//초기 N마리 소환
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
    }
}

void ACooperationGameMode::HandlePlayerKilled(AActor* DeadPlayer, AController* Killer)
{
    CurrentPlayerCount--;

    ActivePlayers.Remove(DeadPlayer); // 알아서 내부에서 찾고 제거함

    //test Code
    if (CurrentPlayerCount <= 0)
    {
        //게임을 실패한거로 종료.
    }

}




//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleMonsterKilled(AActor* DeadMonster, AController* Killer)
{
    CurrentMonsterCount--;

    ActiveMonsters.Remove(DeadMonster); // 알아서 내부에서 찾고 제거함

    if (CurrentMonsterCount <= 0)
    {
        EndStage1();
    }
}


//클리어 트리거를 가지고 있는 오브젝트의 트리거가 눌리면
void ACooperationGameMode::TriggerStage1Clear(UObject* Object)
{
    if (Object == Stage1ClearTriggerObject[0])
    {
        Stage1ClearTrigger1 = true;
    }
    else if(Object == Stage1ClearTriggerObject[1])
    {
        Stage1ClearTrigger2 = true;
    }

    if (Stage1ClearTrigger1 && Stage1ClearTrigger2)
    {
        EndStage1();
    }
}









//적 AI가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleEnemyKilled(AActor* DeadMonster, AController* Killer)
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
void ACooperationGameMode::HandleBossMonsterKilled(AController* Killer)
{
    EndStage3();
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
        APlayerController* PC = Cast<APlayerController>(*It);
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
                UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn: %s"), *GetNameSafe(SpawnedCharacter));
            }
        }
    }

    // SpawnedCharacters 배열로 생성된 캐릭터들을 확인할 수 있습니다.
    for (ABaseWitch* Character : SpawnedCharacters)
    {
        UE_LOG(LogTemp, Warning, TEXT("Managed Character: %s"), *GetNameSafe(Character));
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
    //방법 2
    /*
    // 레벨 서브시스템에 레벨 변경 알림
    UMyLevelSubsystem* LevelSubsystem = GetWorld()->GetSubsystem<UMyLevelSubsystem>();
    if (LevelSubsystem)
    {
        LevelSubsystem->OnLevelChanged("NewLevel");
    }
    */
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


//납치하면 상대할 보스를 소환해놓기?
void ACooperationGameMode::SpawnGhostBoss()
{

}