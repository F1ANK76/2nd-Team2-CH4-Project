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
    CooperationGameState = GetGameState<ACooperationGameState>();

    StageIndex = 1;

    InitPlayerUI();
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            StartGame();
        }), 5.0f, false);
}


//플레이어 컨트롤러 -> 인스턴스에 담아두고, 필요할 때 생성. UISubSystem이 호출해서 위젯 만들고 -> UPdate 함수 호출은 게임모드


void ACooperationGameMode::StartGame()
{
    //로딩되고 스테이지를 본격적으로 시작하기 전 처리할 함수...
    //게임 준비, 시작 UI를 띄우고...

    if (IsValid(CooperationGameState))
    {

    }


    //타이머 작동 시키고...
    if (IsValid(CooperationGameState))
    {

    }




    MoveNextStage();
}

void ACooperationGameMode::EndGame()
{
    

    //result Page Open
}

//Stage1 준비 트리거
void ACooperationGameMode::ReadyStage1()
{
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);
    //몬스터 소환하면서,
    SpawnMonsters();
    SetPlayerLocation();
    //게임모드 
    //몬스터 못움직이게 잠시 세팅값 조정.

    // 캐릭터도 못움직이게 동결 시켜놓고.

    //CooperationUI Stage1로 바꾸기
    //->ActiveStage1Widget();

    //Game Start UI Open까지 대기
    //Game Start UI 끝나면



    //여긴 바로 시작... 버프 선택 없음!
    //몬스터, 캐릭터 움직임 동결되어있는 것 풀면서 시작.
    StartStage1();
}

//Stage2 준비 트리거
void ACooperationGameMode::ReadyStage2()
{
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


    // 버프 완료 되었는지 확인하기
    CheckUntilAllPlayerSelectBuff();

}

//Stage3 준비 트리거
void ACooperationGameMode::ReadyStage3()
{
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);

    SetPlayerLocation();

    //보스 못 움직이게 잠시 세팅값 조정.

    // 캐릭터도 못움직이게 동결 시켜놓고.

    //Game Start UI Open까지 대기
    //Game Start UI 끝나면
    // 
    // 
    //CooperationUI Stage3로 바꾸기
    //->ActiveStage3Widget();



    CheckUntilAllPlayerSelectBuff();
}

//Stage1 시작 트리거
void ACooperationGameMode::StartStage1()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);

    //몬스터 움직임 동결도 해제
}
//Stage2 시작 트리거
void ACooperationGameMode::StartStage2()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);

    //적 AI 움직임 동결도 해제
}
//Stage3 시작 트리거
void ACooperationGameMode::StartStage3()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);
    
    //보스 몬스터 활동 개시

    if (IsValid(CooperationGameState))
    {
        //CooperationGameState->TurnOnTimer();
    }
}
//Stage1 종료 트리거
void ACooperationGameMode::EndStage1()
{
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}

//Stage2 종료 트리거
void ACooperationGameMode::EndStage2()
{
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}
//Stage3 종료 트리거
void ACooperationGameMode::EndStage3()
{


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
    //게임 스테이트 보고 좀 열어달라고 요청하기
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->RequestPlayerToOpenBuffUI();
    }
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
    if (!IsValid(CooperationGameState)) return;

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
}



void ACooperationGameMode::SetPlayerUnReady(ACharacter* PlayerChar)
{
    if (!PlayerChar) return;

    APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController());
    if (AWitchController* MyPC = Cast<AWitchController>(PC))
    {
        //MyPC->bCanControl = false;
    }
}

void ACooperationGameMode::SetPlayerReady(ACharacter* PlayerChar)
{
    if (!PlayerChar) return;

    APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController());
    if (AWitchController* MyPC = Cast<AWitchController>(PC))
    {
        //MyPC->bCanControl = true;
        //GetPawn()->DisableInput(this); 이라는 기능이 있다고 합니다....
    }
}





void ACooperationGameMode::SpawnEnemies()
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
            ActiveMonsters.Add(SpawnLocation, SpawnedMonster);
            CurrentMonsterCount++;
        }
    }
}



//초기 N마리 소환
void ACooperationGameMode::SpawnMonsters()
{
    UWorld* World = GetWorld();
    if (!World || !EnemyBlueprintClass || EnemySpawnLocations.Num() == 0) return;

    for (const FVector& SpawnLocation : MonsterSpawnLocations)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(EnemyBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedMonster)
        {
            ActiveEnemies.Add(SpawnLocation, SpawnedMonster);
            CurrentEnemyCount++;
        }
    }
}

//몬스터가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleMonsterKilled(AController* Killer)
{
    
}

//적 AI가 죽을 때 누구한테 죽었는지 정보를 넘기며 게임모드에 알려주면 호출되는 함수.
void ACooperationGameMode::HandleEnemyKilled(AController* Killer)
{
    //조건은 제대로 지정해야할듯
    CurrentEnemyCount--;
    if (CurrentEnemyCount <= 0)
    {
        EndStage2();
    }
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
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            // 컨트롤러에서 게임인스터스에다가 UI 띄우는 함수 호출하라고 명령
            //PlayerController->ShowUI(EAddWidgetType::CooperationWidget);
        }
    }
}