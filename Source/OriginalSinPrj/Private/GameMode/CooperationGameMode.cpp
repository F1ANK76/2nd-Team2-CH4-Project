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
    CooperationGameState = GetGameState<ACooperationGameState>();

    StageIndex = 1;

    InitPlayerUI();
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            StartGame();
        }), 5.0f, false);
}


//�÷��̾� ��Ʈ�ѷ� -> �ν��Ͻ��� ��Ƶΰ�, �ʿ��� �� ����. UISubSystem�� ȣ���ؼ� ���� ����� -> UPdate �Լ� ȣ���� ���Ӹ��


void ACooperationGameMode::StartGame()
{
    //�ε��ǰ� ���������� ���������� �����ϱ� �� ó���� �Լ�...
    //���� �غ�, ���� UI�� ����...

    if (IsValid(CooperationGameState))
    {

    }


    //Ÿ�̸� �۵� ��Ű��...
    if (IsValid(CooperationGameState))
    {

    }




    MoveNextStage();
}

void ACooperationGameMode::EndGame()
{
    

    //result Page Open
}

//Stage1 �غ� Ʈ����
void ACooperationGameMode::ReadyStage1()
{
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);
    //���� ��ȯ�ϸ鼭,
    SpawnMonsters();
    SetPlayerLocation();
    //���Ӹ�� 
    //���� �������̰� ��� ���ð� ����.

    // ĳ���͵� �������̰� ���� ���ѳ���.

    //CooperationUI Stage1�� �ٲٱ�
    //->ActiveStage1Widget();

    //Game Start UI Open���� ���
    //Game Start UI ������



    //���� �ٷ� ����... ���� ���� ����!
    //����, ĳ���� ������ ����Ǿ��ִ� �� Ǯ�鼭 ����.
    StartStage1();
}

//Stage2 �غ� Ʈ����
void ACooperationGameMode::ReadyStage2()
{
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


    // ���� �Ϸ� �Ǿ����� Ȯ���ϱ�
    CheckUntilAllPlayerSelectBuff();

}

//Stage3 �غ� Ʈ����
void ACooperationGameMode::ReadyStage3()
{
    SetPlayerUnReady(SpawnedCharacters[0]);
    SetPlayerUnReady(SpawnedCharacters[1]);

    SetPlayerLocation();

    //���� �� �����̰� ��� ���ð� ����.

    // ĳ���͵� �������̰� ���� ���ѳ���.

    //Game Start UI Open���� ���
    //Game Start UI ������
    // 
    // 
    //CooperationUI Stage3�� �ٲٱ�
    //->ActiveStage3Widget();



    CheckUntilAllPlayerSelectBuff();
}

//Stage1 ���� Ʈ����
void ACooperationGameMode::StartStage1()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);

    //���� ������ ���ᵵ ����
}
//Stage2 ���� Ʈ����
void ACooperationGameMode::StartStage2()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);

    //�� AI ������ ���ᵵ ����
}
//Stage3 ���� Ʈ����
void ACooperationGameMode::StartStage3()
{
    SetPlayerReady(SpawnedCharacters[0]);
    SetPlayerReady(SpawnedCharacters[1]);
    
    //���� ���� Ȱ�� ����

    if (IsValid(CooperationGameState))
    {
        //CooperationGameState->TurnOnTimer();
    }
}
//Stage1 ���� Ʈ����
void ACooperationGameMode::EndStage1()
{
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}

//Stage2 ���� Ʈ����
void ACooperationGameMode::EndStage2()
{
    StageIndex++;
    RequestTurnOnBuffSelectUI();

    MoveNextStage();

}
//Stage3 ���� Ʈ����
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
    //���� ������Ʈ ���� �� ����޶�� ��û�ϱ�
    if (IsValid(CooperationGameState))
    {
        CooperationGameState->RequestPlayerToOpenBuffUI();
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
        //GetPawn()->DisableInput(this); �̶�� ����� �ִٰ� �մϴ�....
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



//�ʱ� N���� ��ȯ
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

//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void ACooperationGameMode::HandleMonsterKilled(AController* Killer)
{
    
}

//�� AI�� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void ACooperationGameMode::HandleEnemyKilled(AController* Killer)
{
    //������ ����� �����ؾ��ҵ�
    CurrentEnemyCount--;
    if (CurrentEnemyCount <= 0)
    {
        EndStage2();
    }
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
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            // ��Ʈ�ѷ����� �����ν��ͽ����ٰ� UI ���� �Լ� ȣ���϶�� ���
            //PlayerController->ShowUI(EAddWidgetType::CooperationWidget);
        }
    }
}