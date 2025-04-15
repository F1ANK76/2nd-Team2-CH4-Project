// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CooperationGameMode.h"
#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"

ACooperationGameMode::ACooperationGameMode()
{
    bUseSeamlessTravel = true; // Seamless Travel Ȱ��ȭ
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
    //�� �̷����� ó���� �ϰ�....
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



void ACooperationGameMode::SetPlayerColorIndex()
{
    Player1ColorIndex = 0; // Ȥ�� ColorIndex::Red ... ColorIndex::White....
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

    //�� ������ �ƴѵ�, ĳ���Ͱ� �����Ѵٰ� ������ ��� -1 ��ȯ.
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
    //�� ������ �ƴѵ�, ĳ���Ͱ� �����Ѵٰ� ������ ��� -1 ��ȯ.
    return -1;
}


void ACooperationGameMode::StartGame()
{
    //Camera Settings
    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[0]);
    
    CooperationGameState->InitPlayerInfo();

    //Functions to be processed before loading and starting the stage
    //Prepare the game, display the start UI...

    //Player Index Assignment
    SetPlayerColorIndex();

    //Set Stage index -> 1
    StageIndex = 1;
    CooperationGameState->CurrentStageIndex = StageIndex;
    //Move Stage -> Prepare Stage 1 
    MoveNextStage();
}


void ACooperationGameMode::EndGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Game End "));

    //�ϴ� �÷��̾� �Է��� ���߱�.
    SetPlayerUnReady();

    SpawnedCharacters[0]->SetActorLocation(PlayerResultLocations[0]);
    SpawnedCharacters[1]->SetActorLocation(PlayerResultLocations[1]);

    RequestOpenResultUI();
    

    TravelLevel();

}

void ACooperationGameMode::TravelLevel()
{
    UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
    if (MyGI)
    {
        // ���ϴ� �Լ��� ���� ��� ����
        MyGI->RequestOpenLevelByType(ELevelType::TitleLevel, false);
    }
    //�����ν��Ͻ��� ���� ����޶�� ȣ���ϱ�.
}


void ACooperationGameMode::RequestOpenResultUI()
{
    UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
    if (MyGI)
    {
        MyGI->ResponseShowWidget();
    }
}

//Stage1 �غ� Ʈ����
void ACooperationGameMode::ReadyStage1()
{
    //Prepare Stage 1

    UE_LOG(LogTemp, Warning, TEXT("Ready Stage1"));

    //Turn off Player Input

    SetPlayerUnReady();

    //Spawn Monster
    SpawnMonsters();

    //Set to player position for stage 1
    SetPlayerLocation();



    //CooperationUI Turn on Stage1 UI
    //->ActiveStage1Widget();
    CooperationGameState->TurnOnStage1Widget();

    //Game Start UI Open
    //When Game Start UI ...

    //Start Stage1
    StartStage1();
}

//Stage2 �غ� Ʈ����
void ACooperationGameMode::ReadyStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage2"));


    SetPlayerLocation();
    //Enemy AI Spawn
    SpawnEnemies();
    // Character Input Block
    SetPlayerUnReady();

    //Game Start UI Open���� ���
    //When Game Start UI Ended...


    //CooperationUI Stage2�� �ٲٱ�
    //CooperationWidget->ActiveStage2Widget();
    CooperationGameState->TurnOnStage2Widget();


    //Test Code
    //Delay for Test

    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &ACooperationGameMode::CheckUntilAllPlayerSelectBuff, 5.0f, false);

    
    // ���� �Ϸ� �Ǿ����� Ȯ���ϱ�
    //CheckUntilAllPlayerSelectBuff();

}

//Stage3 �غ� Ʈ����
void ACooperationGameMode::ReadyStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage3"));
    SetPlayerUnReady();

    SetPlayerLocation();
    SpawnBossMonsters();

    //���� �� �����̰� ��� ���ð� ����.

    // ĳ���͵� �������̰� ���� ���ѳ���.

    //Game Start UI Open���� ���
    //Game Start UI ������
    // 
    // 
    //CooperationUI Stage3�� �ٲٱ�
    CooperationGameState->TurnOnStage3Widget();

    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage3Widget();
        }
    }



    CooperationGameState->SetStage3CameraTransform();

    AttachPlayerToCamera(SpawnedCharacters[0], SpawnedBaseCamera[0]);
    AttachPlayerToCamera(SpawnedCharacters[1], SpawnedBaseCamera[1]);
    //Test Code
    //�ӽ� ������ ���

    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &ACooperationGameMode::CheckUntilAllPlayerSelectBuff, 5.0f, false);


    // ���� �Ϸ� �Ǿ����� Ȯ���ϱ�
    //CheckUntilAllPlayerSelectBuff();
}

//Stage1 ���� Ʈ����
void ACooperationGameMode::StartStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage1"));

    //Turn On Player Input
    SetPlayerReady();

    //Monster Movement...
}

//Stage2 ���� Ʈ����
void ACooperationGameMode::StartStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage2"));

    //Turn On Player Input
    SetPlayerReady();

    //����, ĳ���� ������ ����Ǿ��ִ� �� Ǯ��. Enemy Movement UnBlock

    //
}

//Stage3 ���� Ʈ����
void ACooperationGameMode::StartStage3()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage3"));

    //Turn On Player Input
    SetPlayerReady();
    
    if (IsValid(CooperationGameState))
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState is valid"));
        CooperationGameState->SetActorTickEnabled(true);
        CooperationGameState->TurnOnTimer();
    }

}

//Stage1 ���� Ʈ����
void ACooperationGameMode::EndStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage1"));
    StageIndex++;
    CooperationGameState->CurrentStageIndex = StageIndex;
    RequestTurnOnBuffSelectUI();
    MoveNextStage();
}

//Stage2 ���� Ʈ����
void ACooperationGameMode::EndStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage2"));
    StageIndex++;
    CooperationGameState->CurrentStageIndex = StageIndex;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}
//Stage3 ���� Ʈ����
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
        //��� �÷��̾ ������ �����ߴ��� üũ �Ǿ�� �ؿ��� ����Ǿ����
        ReadyStage2();
        break;
    case 3:
        //��� �÷��̾ ������ �����ߴ��� üũ �Ǿ�� �ؿ��� ����Ǿ����
        ReadyStage3();
        break;
    default:

        break;
    }
}//�� Stage ���� �� �Ų����� ����� ��ȯ�ϱ� ���� �Լ� �ʿ�


void ACooperationGameMode::RequestTurnOnBuffSelectUI()
{

    UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance());
    if (MyGI)
    {
        //MyGI->OpenWidget(BuffSelect);
    }
}
    
void ACooperationGameMode::ApplyBuffToBothPlayer()
{
    //���� �ΰ� ������� ���� �����϶�� ��û.
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

    /* //�׽�Ʈ��... �ּ� ���� �ؾߵ�
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


void ACooperationGameMode::SetPlayerUnReady()
{
    CooperationGameState->SetPlayerMove(false);
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

//�ʱ� N���� ��ȯ
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

//���� ��ȯ
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

    ActivePlayers.Remove(DeadPlayer); // �˾Ƽ� ���ο��� ã�� ������

    //test Code
    if (CurrentPlayerCount <= 0)
    {
        //������ �����Ѱŷ� ����.
    }
}


//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void ACooperationGameMode::HandleMonsterKilled(AActor* DeadMonster, AController* Killer)
{
    CurrentMonsterCount--;

    ActiveMonsters.Remove(DeadMonster); // �˾Ƽ� ���ο��� ã�� ������

    if (CurrentMonsterCount <= 0)
    {
        EndStage1();
    }
}


//Ŭ���� Ʈ���Ÿ� ������ �ִ� ������Ʈ�� Ʈ���Ű� ������
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


//�� AI�� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void ACooperationGameMode::HandleEnemyKilled(AActor* DeadMonster, AController* Killer)
{
    //������ ����� �����ؾ��ҵ�
    CurrentEnemyCount--;
    // ���� ���͸� ActiveMonsters ��Ͽ��� ����
    ActiveEnemies.Remove(DeadMonster); // �˾Ƽ� ���ο��� ã�� ������
    
    if (CurrentEnemyCount <= 0)
    {
        EndStage2();
    }
}

//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void ACooperationGameMode::HandleBossMonsterKilled(AController* Killer)
{
    EndStage3();
}

void ACooperationGameMode::PostSeamlessTravel()
{
    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
    Super::PostSeamlessTravel();  // �⺻ SeamlessTravel ó��

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
    
    //Game Stage���� �ʿ��� ����� ���� ���� ������Ʈ�� ��Ʈ�ѷ� ����صα�
    CooperationGameState->RegisterInitialController(PC);
}

void ACooperationGameMode::SpawnPlayers()
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
                ActivePlayers.Add(SpawnedCharacter);
                CurrentPlayerCount++;
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
        MyGI->ResponseShowWidget();
    }
}


void ACooperationGameMode::BossSetPlayerLocation(ACharacter* PlayerChar)
{
    if (IsValid(PlayerChar))
    {
        //Camera Settings
        AttachPlayerToCamera(PlayerChar, SpawnedBaseCamera[1]);
        // �÷��̾��� ������ �� �ұ� ����...
        PlayerHijackedLocation = PlayerChar->GetActorLocation();

        PlayerChar->SetActorLocation(BossHijackingLocation[0]);
    }
}

void ACooperationGameMode::BossReturnPlayerLocation(ACharacter* PlayerChar)
{
    if (IsValid(PlayerChar))
    {
        // �÷��̾��� ������ �� �ұ� ����...

        //���� ��ġ �����ؼ� �ǵ�����
        PlayerChar->SetActorLocation(PlayerHijackedLocation);
        AttachPlayerToCamera(PlayerChar, SpawnedBaseCamera[0]);
    }
}


//��ġ�ϸ� ����� ������ ��ȯ�س���?
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
    if (PlayerController == nullptr) return;  // ��Ʈ�ѷ��� ������ ����

    if (PlayerController)
    {
        Camera->ActivateCamera(PlayerController);
    }

}