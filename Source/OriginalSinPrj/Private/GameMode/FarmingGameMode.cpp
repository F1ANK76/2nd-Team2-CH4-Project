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
    bUseSeamlessTravel = true; // Seamless Travel Ȱ��ȭ
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
        InitPlayerUI();
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                StartGame();
            }), 5.0f, false);
    }
    else
    {
        InitPlayerUI();
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                StartGame();
            }), 5.0f, false);
    }
}




void AFarmingGameMode::StartGame()
{

    //�غ�, ���� UI�� ����...
    
    //���� ��ȣ�� ����...
    
    if (FarmingGameState)
    {
        FarmingGameState->StartFarmingMode();
    }
    //���� ��ȯ�ϸ鼭,
    SpawnInitialMonsters();

    //Ÿ�̸� �۵� ��Ű��...
    if (FarmingGameState)
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

void AFarmingGameMode::SpawnMissingMonsters()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Spawned"));
    UWorld* World = GetWorld();
    if (!World || !MonsterBlueprintClass) return;

    for (const FVector& SpawnLocation : MonsterSpawnLocations)
    {
        AActor** FoundMonsterPtr = ActiveMonsters.Find(SpawnLocation);
        bool bNeedToSpawn = false;

        if (FoundMonsterPtr == nullptr)
        {
            bNeedToSpawn = true;
        }
        else
        {
            AActor* FoundMonster = *FoundMonsterPtr;
            if (!IsValid(FoundMonster))
            {
                bNeedToSpawn = true;
            }
        }

        if (bNeedToSpawn)
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
}


//���Ͱ� ���� �� �������� �׾����� ������ �ѱ�� ���Ӹ�忡 �˷��ָ� ȣ��Ǵ� �Լ�.
void AFarmingGameMode::HandleMonsterKilled(AController* Killer)
{
    CurrentMonsterCount--;

    // ���� ���͸� ActiveMonsters ��Ͽ��� ����
    for (auto It = ActiveMonsters.CreateIterator(); It; ++It)
    {
        AActor* Monster = It.Value();
        
        if (!IsValid(Monster))
        {
            It.RemoveCurrent();
            break;
        }
    }

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

void AFarmingGameMode::HandleFarmingModeEnded()
{
    // ���� ��ȯ ó�� (�� �̵�, ���� �ܰ� �غ� ��)
}

//�÷��̾ ������ �α����ϸ� ����Ǵ� �Լ�.
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


void AFarmingGameMode::PostSeamlessTravel()
{
    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel Called"));
    Super::PostSeamlessTravel();  // �⺻ SeamlessTravel ó��

    NetMode = GetNetMode(); // ��Ƽ �̱� ���� �б�
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