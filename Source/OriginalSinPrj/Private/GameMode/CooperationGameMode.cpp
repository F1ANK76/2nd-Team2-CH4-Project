// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CooperationGameMode.h"
#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"
#include "GameFramework/PlayerController.h"
#include "Player/Controller/WitchController.h"


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

    //�ϴ� �÷��̾� �Է��� ���߱�.
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

//Stage1 �غ� Ʈ����
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

//Stage2 �غ� Ʈ����
void ACooperationGameMode::ReadyStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Ready Stage2"));
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);
    SpawnEnemies();
    SetPlayerLocation();
    //���� �������̰� ��� ���ð� ����.

    // ĳ���͵� �������̰� ���� ���ѳ���.

    //Game Start UI Open���� ���
    //Game Start UI ������


    //CooperationUI Stage2�� �ٲٱ�
    //->ActiveStage2Widget();

    //����, ĳ���� ������ ����Ǿ��ִ� �� Ǯ��.




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
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);

    SetPlayerLocation();
    SpawnBossMonsters();

    //���� �� �����̰� ��� ���ð� ����.

    // ĳ���͵� �������̰� ���� ���ѳ���.

    //Game Start UI Open���� ���
    //Game Start UI ������
    // 
    // 
    //CooperationUI Stage3�� �ٲٱ�
    //->ActiveStage3Widget();
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
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->TurnOnTimer();
    }
    //Monster Movement...
}

//Stage2 ���� Ʈ����
void ACooperationGameMode::StartStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Stage2"));

    //Turn On Player Input
    //SetPlayerReady(SpawnedCharacters[0]);
    //SetPlayerReady(SpawnedCharacters[1]);

    //Enemy Movement...
}

//Stage3 ���� Ʈ����
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

//Stage1 ���� Ʈ����
void ACooperationGameMode::EndStage1()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage1"));
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}

//Stage2 ���� Ʈ����
void ACooperationGameMode::EndStage2()
{
    UE_LOG(LogTemp, Warning, TEXT("End Stage2"));
    StageIndex++;
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
    /*
    //���� ������Ʈ ���� �� ����޶�� ��û�ϱ�
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->RequestPlayerToOpenBuffUI();
    }
    */
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


/*
UFUNCTION(Client, Reliable)
void Client_DisableInput();

void AWitchController::Client_DisableInput_Implementation()
{
    DisableInput(this); // this�� Local PlayerController
}
*/

/*
AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
if (CharacterController)
{
    CharacterController->Client_DisableInput(); // Ŭ���̾�Ʈ���� �Է� ��Ȱ��
}
*/



void ACooperationGameMode::SetPlayerUnReady(ACharacter* PlayerChar)
{
    if (!PlayerChar) return;
    AWitchController* CharacterController = Cast<AWitchController>(PlayerChar->GetController());
    //CharacterController->Client_DisableInput(); // Ŭ���̾�Ʈ���� �Է� ��Ȱ��
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

//�ʱ� N���� ��ȯ
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
    //��� 2
    /*
    // ���� ����ý��ۿ� ���� ���� �˸�
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