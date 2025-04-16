// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/WitchController.h"
#include "../GameInstance/DataSubsystem.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"


ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false); // �ϴ� ���α�

    bIsStage3Started = false;
    Timer = 0.0f;
    bReplicates = true;

    CameraLocation = FVector(-400.f, 200.f, 0.f);
    CameraRotation = FRotator::ZeroRotator;
    CameraDistance = 0;    
}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // ���� ������Ʈ Ŭ�������� ���� ��忡 ����
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);
    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));
}
void ACooperationGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (IsValid(CooperationGameGameMode))
    {
        switch (CooperationGameGameMode->StageIndex)
        {
        case 1:

            SetStage1CameraTransform();
            break;

        case 2:
            SetStage2CameraTransform();
            break;

        case 3:
            //SetStage3CameraTransform(); -> Run only once at Start Stage3  -> in Gamemode
            break;

        default:

            break;
        }
    }
    

    if (HasAuthority() && bIsStage3Started)
    {
        Timer += DeltaSeconds;
        UpdateTimer();
    }
}



//ShowLevelWidget


//������ Ÿ�̸� �ѱ�
void ACooperationGameState::TurnOnTimer()
{
    bIsStage3Started = true;
}

void ACooperationGameState::TurnOffTimer()
{
    bIsStage3Started = false;
}


void ACooperationGameState::TurnOnStage1Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage1Widget();
        }
    }
}
void ACooperationGameState::TurnOnStage2Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}
void ACooperationGameState::TurnOnStage3Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage3Widget();
        }
    }
}

void ACooperationGameState::TurnOffStage3Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->DeactivateAllWidgets();
        }
    }
}

void ACooperationGameState::CreateBuffSelectUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Buff Widget"));
            UISubsystem->ShowWidget(EAddWidgetType::BuffSelectWidget);
            UISubsystem->SetMouseMode(true);
            bIsPlayerBuffSelect = 0;
        }
    }
}



void ACooperationGameState::OnRep_TurnOffBuffUI()
{
    CloseBuffSelectUI();
}


void ACooperationGameState::CloseBuffSelectUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Buff Widget"));            
            UISubsystem->CloseWidget(EAddWidgetType::BuffSelectWidget);
            UISubsystem->SetMouseMode(false);
        }
    }
}


void ACooperationGameState::OnRep_TurnOnStageUI()
{
    if (CurrentStageIndex == 1)
    {
        TurnOnStage1Widget();
    }
    else if (CurrentStageIndex == 2)
    {
        CreateBuffSelectUI();
        TurnOnStage2Widget();
    }
    else if (CurrentStageIndex == 3)
    {
        CreateBuffSelectUI();
        TurnOnStage3Widget();
    }
    else if (CurrentStageIndex == 4)
    {
        TurnOffStage3Widget();
        TurnOnResultWidget();
    }
}

void ACooperationGameState::TurnOnResultWidget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Show Result Widget"));
            UISubsystem->ShowWidget(EAddWidgetType::ResultWidget);
            // ���⼭ UISubsystem ��� ����!
            //Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}

void ACooperationGameState::InitPlayerInfo()
{
    if (HasAuthority())  // ���������� ����Ǵ� �ڵ�
    {
        PlayerInfos.Empty(); // ���� ���� ����
        //��𼱰� �޾ƿ;���...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr, 
        100.0f, 100.0f, 
        0, 0, 
        0, 0.0f, 
        0, 
        0, 
        0, 100 });
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];
        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100 });
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
    }
    InitPlayerUIInfo();

}

void ACooperationGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{  
    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    
    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[0])
    {
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxHP = State.MaxHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxEXP = State.MaxEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].MaxMana = State.MaxMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].LifePoint = State.LifePoint;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].AirbornePercent = State.AirbornePercent;
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];
        CooperationGameGameMode->RequestUpdateUI(0);
    }
    
    
    
    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[1])
    {
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxHP = State.MaxHP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxEXP = State.MaxEXP;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].MaxMana = State.MaxMana;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].LifePoint = State.LifePoint;
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].AirbornePercent = State.AirbornePercent;
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
        CooperationGameGameMode->RequestUpdateUI(1);
    }
    
    
}


void ACooperationGameState::InitPlayerUIInfo()
{
    //���� �����ؼ� ����
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void ACooperationGameState::UpdatePlayerUIInfo()
{
    //���� �����ؼ� ����
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdatePlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}


void ACooperationGameState::ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype)
{
    if (IsValid(CooperationGameGameMode))
    {
        if (CooperationGameGameMode->StageIndex == 2)
        {

        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {

        }
    }

    if (player == PlayerControllerSet[0]) //<- �̰� �³�?
    {
        Player1Stage1SelectedBuff = Bufftype;

    }

    if (player == PlayerControllerSet[1])
    {
        Player2Stage1SelectedBuff = Bufftype;
    }

    
}

void ACooperationGameState::ApplyBuffStat()
{
    if (IsValid(CooperationGameGameMode))
    {
        if (CooperationGameGameMode->StageIndex == 2)
        {
            /*
            PlayerInfo += 
            SelectedBuff[0];
            SelectedBuff[1];
            */
        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {
            /*
            PlayerInfo += 
            SelectedBuff[2];
            SelectedBuff[3];
            */
        }
    }
    //UI ����� ���

    bIsPlayerBuffSelect = 0;
}

void ACooperationGameState::AddExperienceToPlayer(AActor* Player, int32 Amount)
{
    if (Player)
    {
        // PlayerInfos���� �÷��̾� ���� ã��
        FPlayerData* PlayerData = PlayerInfos.Find(Player);

        if (PlayerData)
        {
            PlayerData->CurrentEXP += Amount;
            CheckLevelUp(Player);
            UpdatePlayerUIInfo(); // UI ����
        }
    }
}

void ACooperationGameState::CheckLevelUp(AActor* Player)
{
    if (Player)
    {
        FPlayerData& Info = PlayerInfos[Player];

        while (Info.CurrentEXP >= 100)
        {
            Info.CurrentEXP -= 100;
            Info.PlayerLevel++;

        }
    }
}


void ACooperationGameState::SetPlayerPawn(ABaseWitch* InPawn)
{
    PlayerPawnRef = InPawn;
}

//���� ��Ʈ�ѷ� ����صα�
void ACooperationGameState::RegisterInitialController(APlayerController* PC)
{
    if (IsValid(PC) && !PlayerControllerSet.Contains(PC))
    {
        PlayerControllerSet.Add(PC);
        UE_LOG(LogTemp, Log, TEXT("Controller Registered: %s"), *PC->GetName());
    }
}



void ACooperationGameState::SetStage1CameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;


    for (AActor* Player : CooperationGameGameMode->ActivePlayers)
    {
        FVector PlayerLocation = Player->GetActorLocation();

        SumPlayerLocation += PlayerLocation;
        if (PlayerLocation.Y > maxY)
        {
            maxY = PlayerLocation.Y;
        }
        if (PlayerLocation.Y < minY)
        {
            minY = PlayerLocation.Y;
        }

        if (PlayerLocation.Z > maxZ)
        {
            maxZ = PlayerLocation.Z;
        }
        if (PlayerLocation.Z < minZ)
        {
            minZ = PlayerLocation.Z;
        }
    }

    FVector MeanPlayerLocation = FVector::ZeroVector;
    if (CooperationGameGameMode->ActivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / CooperationGameGameMode->ActivePlayers.Num();
    }

    if (HasAuthority()) // �������� Ȯ��
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);

        CameraLocation = MeanPlayerLocation;
        CameraRotation = FRotator::ZeroRotator;
    }

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
    
}

void ACooperationGameState::SetStage2CameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    FVector SumEnemyLocation = FVector::ZeroVector;

    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;

    for (AActor* Player : CooperationGameGameMode->ActivePlayers)
    {
        FVector PlayerLocation = Player->GetActorLocation();

        SumPlayerLocation += PlayerLocation;
        if (PlayerLocation.Y > maxY)
        {
            maxY = PlayerLocation.Y;
        }
        if (PlayerLocation.Y < minY)
        {
            minY = PlayerLocation.Y;
        }

        if (PlayerLocation.Z > maxZ)
        {
            maxZ = PlayerLocation.Z;
        }
        if (PlayerLocation.Z < minZ)
        {
            minZ = PlayerLocation.Z;
        }
    }

    for (AActor* Enemy : CooperationGameGameMode->ActiveEnemies)
    {
        if (IsValid(Enemy))
        {
            FVector EnemyLocation = Enemy->GetActorLocation();

            SumEnemyLocation += EnemyLocation;

            if (EnemyLocation.Y > maxY)
            {
                maxY = EnemyLocation.Y;
            }
            if (EnemyLocation.Y < minY)
            {
                minY = EnemyLocation.Y;
            }

            if (EnemyLocation.Z > maxZ)
            {
                maxZ = EnemyLocation.Z;
            }
            if (EnemyLocation.Z < minZ)
            {
                minZ = EnemyLocation.Z;
            }
        }
    }

    FVector MeanActorLocation = FVector::ZeroVector;

    int32 NumOfActors = CooperationGameGameMode->ActivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num();

    if (CooperationGameGameMode->ActivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num() > 0)
    {
        MeanActorLocation = (SumPlayerLocation + SumEnemyLocation) / NumOfActors;
    }


    if (HasAuthority())
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);
        CameraLocation = MeanActorLocation;
        CameraRotation = FRotator::ZeroRotator;

    }// �������� Ȯ��

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}

void ACooperationGameState::SetStage3CameraTransform()
{

    //Fix Camera Settings
    CameraDistance = CooperationGameGameMode->BossStageCameraDistance[0];
    CameraLocation = CooperationGameGameMode->BossStageCameraLocations[0];
    CameraRotation = CooperationGameGameMode->BossStageCameraAngle[0];

    CooperationGameGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}


//�������� ��ġ���Ͽ� ���� ����

//








////////////////////////////��Ƽ ó��

void ACooperationGameState::OnRep_UpdatePlayer1DataUI()
{
    UpdatePlayerUIInfo();
}

void ACooperationGameState::OnRep_UpdatePlayer2DataUI()
{
    UpdatePlayerUIInfo();
}


void ACooperationGameState::OnRep_UpdatePlayerInitData()
{
    InitPlayerUIInfo();
}


void ACooperationGameState::UpdateTimer()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossTimer(Timer);
        }
    }
}

void ACooperationGameState::OnRep_UpdateTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), Timer);
    UpdateTimer();
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACooperationGameState, CameraLocation);
    DOREPLIFETIME(ACooperationGameState, CameraRotation);
    DOREPLIFETIME(ACooperationGameState, CameraDistance);
    DOREPLIFETIME(ACooperationGameState, Timer); 
    DOREPLIFETIME(ACooperationGameState, bIsPlayerCanMove); 
    DOREPLIFETIME(ACooperationGameState, CurrentStageIndex);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerBuffSelect);
    DOREPLIFETIME(ACooperationGameState, SelectBuffPlayer);
    DOREPLIFETIME(ACooperationGameState, Player1DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player2DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player1StateData);
    DOREPLIFETIME(ACooperationGameState, Player2StateData);
    DOREPLIFETIME(ACooperationGameState, PlayerDataChanged);
}


void ACooperationGameState::OnRep_SetPlayerMove()
{
    UE_LOG(LogTemp, Log, TEXT("This is NOT the local player controller."));
    UWorld* WorldContext = GetWorld();
    for (FConstPlayerControllerIterator It = WorldContext->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            if (bIsPlayerCanMove)
            {
                PC->GetPawn()->DisableInput(PC);
            }
            else
            {
                PC->GetPawn()->EnableInput(PC);
            }
        }
    }
    
}


void ACooperationGameState::SetPlayerMove(bool bCanMove)
{
    bIsPlayerCanMove = bCanMove;

    if (bIsPlayerCanMove)
    {
        bIsPlayerCanMove = false;
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
    else
    {
        bIsPlayerCanMove = true;
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
}


void ACooperationGameState::OnRep_UpdateBossDataUI()
{
    UpdateBossDataUI();
}

void ACooperationGameState::UpdateBossDataUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossUI(BossData[0]);
        }
    }
}


void ACooperationGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    Multicast_ApplyDamage(Attacker, Damage, HitLocation);
}

void ACooperationGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) 
{
    Multicast_TakeDamage(Victim, Damage, HitLocation);
}

void ACooperationGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) 
{
    Multicast_OnDeathPlayer(Player, DeathLocation);

}

void ACooperationGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation) 
{
    Multicast_OnDeathMonster(Monster, DeathLocation);
}



void ACooperationGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    // ��� Ŭ���̾�Ʈ���� �����
    if (IsValid(Attacker))
    {
        // ��: �ǰ� ��ġ�� ����Ʈ ����
        // UGameplayStatics::SpawnEmitterAtLocation(...);
    }

    // ���� ������ ó���� �ʿ��ϴٸ� ���⼭ ���� (�Ǵ� ���������� �� ���� ����)
}

void ACooperationGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{
    if (IsValid(Victim))
    {
        // ��: ���� ���� �ִϸ��̼� ���
    }
   
    /*
    Victim.HP -= Damage;
    If(Victim.HP <= MaxHP)
    {
        if(Victim == Player)
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
        if Victim == enemy
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
        if Victim == monster
            Multicast_OnDeathMonster_Implementation(Victim, HitLocation);
    }

    */
}

void ACooperationGameState::Multicast_OnDeathPlayer_Implementation(ACharacter* Player, const FVector& DeathLocation)
{
    if (IsValid(Player))
    {
        /*
        if (Victim == Player)
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
            playDeathAnim
            if Victim.Life > 0
                Respawn
            else
                HandlePlayerKilled

        if Victim == enemy
            Multicast_OnDeathPlayer_Implementation(Victim, HitLocation);
            GameMode->HandleEnemyKilled(Monster, AController* Killer)
        */

        // ��: ��� �ִϸ��̼�, ȿ�� ��
        // Player->PlayDeathAnimation(); ���� �Լ� ȣ��
    }
}

void ACooperationGameState::Multicast_OnDeathMonster_Implementation(AActor* Monster, const FVector& DeathLocation)
{
    if (IsValid(Monster))
    {
        /*
        

        if monster?
        GameMode->HandleMonsterKilled(Monster, AController* Killer)



        */
        

    }
}


//��������Ʈ�� ��ε� ĳ��Ʈ
//���Ӹ�忡�� Adddynamic���� �޾Ƽ� ����./