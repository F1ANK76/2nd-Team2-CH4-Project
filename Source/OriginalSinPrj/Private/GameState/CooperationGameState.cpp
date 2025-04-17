// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/WitchController.h"
#include "../GameInstance/DataSubsystem.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"

#include "OriginalSinPrj/GameInstance/AudioSubsystem.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/BossAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/MonsterAudioDataStruct.h"
#include "Components/AudioComponent.h"


ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = true;

    bIsStage3Started = false;
    SpendedStage3Timer = 0.0f;
    bReplicates = true;

    CameraLocation = FVector(-400.f, 200.f, 0.f);
    CameraRotation = FRotator::ZeroRotator;
    CameraDistance = 0;
}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();


    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);
    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));


    InitCharacterSounds();
    InitBossSounds();
    InitMonsterSounds();

}
void ACooperationGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority() && bIsStage1Started)
    {
        bIsStage1Reached = true;
        SpendedStage1Timer += DeltaSeconds;
    }
    if (HasAuthority() && bIsStage2Started)
    {
        bIsStage2Reached = true;
        SpendedStage2Timer += DeltaSeconds;
    }
    if (HasAuthority() && bIsStage3Started)
    {
        bIsStage3Reached = true;
        SpendedStage3Timer += DeltaSeconds;
        UpdateTimer();
    }
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
            SetStage3CameraTransform();
            break;

        default:

            break;
        }
    }



}



//ShowLevelWidget


//       Ÿ ̸   ѱ 
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
        if (IsValid(MyGI))
        {
            if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
            {
                Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage1Widget();
            }
        }
    }
}
void ACooperationGameState::TurnOnStage2Widget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
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
            UE_LOG(LogTemp, Warning, TEXT("Buff Widget assign?"));
            if (UISubsystem->BuffSelectWidget->IsInViewport())
            {
                TArray<EBuffType> BuffList = BuffUIInit();
                Cast<UBuffSelectWidget>(UISubsystem->BuffSelectWidget)->InitializeBuffs(BuffList);
                UE_LOG(LogTemp, Warning, TEXT("Buff Widget Init"));
            }


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


TArray<EBuffType> ACooperationGameState::BuffUIInit()
{
    //               ⼭     .
//              ϵǾ  ִ         ޾Ƽ ,   ġ    ʰ          ޾            ø   

    TArray<EBuffType> BuffArray =
    {
        EBuffType::ManaUp,
        EBuffType::AttackUp,
        EBuffType::CircleUp ,
        EBuffType::LifePointUp,
        EBuffType::DefenseUp,
        EBuffType::AttackSpeedUp,
        EBuffType::AvoidanceUp,
        EBuffType::AttackRangeUp,
        EBuffType::EnemyCircleDown
    };

    TArray<EBuffType> RequestBuffList;
    if (BuffArray.Num() > 0)
    {
        int RandomIndex = FMath::RandRange(0, BuffArray.Num() - 1);
    }

    for (int i = 0; i < 3; i++)
    {
        int32 Index = FMath::RandRange(0, BuffArray.Num() - 1);
        RequestBuffList.Add(BuffArray[Index]);
        BuffArray.RemoveAt(Index);
    }


    return RequestBuffList;
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

            if (UISubsystem->ResultWidget->IsInViewport())
            {
                Cast<UResultWidget>(UISubsystem->ResultWidget)->InitResultWidgetData(
                    Player1ReceivedDamage,
                    Player2ReceivedDamage,
                    Player1DeathCount,
                    Player2DeathCount,
                    Player1ApplyAttackCount,
                    Player2ApplyAttackCount,
                    SpendedStage1Timer,
                    SpendedStage2Timer,
                    SpendedStage3Timer,
                    bIsStage1Reached,
                    bIsStage2Reached,
                    bIsStage3Reached);
            }

            UISubsystem->SetMouseMode(true);
            //    ⼭ UISubsystem         !
            //Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->ActiveStage2Widget();
        }
    }
}

void ACooperationGameState::InitPlayerInfo()
{
    if (HasAuthority())  //                Ǵ   ڵ 
    {
        PlayerInfos.Empty(); //               
        //  𼱰   ޾ƿ;   ...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player1StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[0]];

        PlayerInfos.Add(CooperationGameGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player2StateData = PlayerInfos[CooperationGameGameMode->ActivePlayers[1]];
        InitPlayerUIInfo();

        Cast<ABaseWitch>(CooperationGameGameMode->ActivePlayers[0])->ResetCharacterState();
        Cast<ABaseWitch>(CooperationGameGameMode->ActivePlayers[1])->ResetCharacterState();
    }
}

void ACooperationGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{

    UE_LOG(LogTemp, Warning, TEXT("Update Info"));

    if (State.OwnWitch == CooperationGameGameMode->ActivePlayers[0])
    {
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP != State.CurrentHP)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player1 HP Updated %f"), (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP - State.CurrentHP));
            Player1ReceivedDamage += (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP - State.CurrentHP);

            PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;

        }
        PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;

        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana != State.CurrentMana)
        {
            Player1ApplyAttackCount++;
            PlayerInfos[CooperationGameGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
        }

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
        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP != State.CurrentHP)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player1 HP Updated %f"), (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP - State.CurrentHP));
            Player2ReceivedDamage += (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP - State.CurrentHP);
            PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
        }
        PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;

        if (PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana != State.CurrentMana)
        {
            Player2ApplyAttackCount++;
            PlayerInfos[CooperationGameGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
        }

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
    //          ؼ      
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            //    ⼭ UISubsystem         !
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void ACooperationGameState::UpdatePlayerUIInfo()
{
    //          ؼ      
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            //    ⼭ UISubsystem         !
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

    if (player == PlayerControllerSet[0]) //<-  ̰   ³ ?
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
    //UI         

    bIsPlayerBuffSelect = 0;
}











void ACooperationGameState::AddExperienceToPlayer(AActor* Player, int32 Amount)
{
    if (Player)
    {
        // PlayerInfos      ÷  ̾       ã  
        FPlayerData* PlayerData = PlayerInfos.Find(Player);

        if (PlayerData)
        {
            PlayerData->CurrentEXP += Amount;
            CheckLevelUp(Player);
            UpdatePlayerUIInfo(); // UI     
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

//       Ʈ ѷ      صα 
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


    for (AActor* Player : CooperationGameGameMode->AlivePlayers)
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
    if (CooperationGameGameMode->AlivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / CooperationGameGameMode->AlivePlayers.Num();
    }

    if (HasAuthority()) //          Ȯ  
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

    for (AActor* Player : CooperationGameGameMode->AlivePlayers)
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

    for (AActor* Enemy : CooperationGameGameMode->AliveEnemies)
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

    int32 NumOfActors = CooperationGameGameMode->AlivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num();

    if (CooperationGameGameMode->AlivePlayers.Num() + CooperationGameGameMode->ActiveEnemies.Num() > 0)
    {
        MeanActorLocation = (SumPlayerLocation + SumEnemyLocation) / NumOfActors;
    }


    if (HasAuthority())
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);
        CameraLocation = MeanActorLocation;
        CameraRotation = FRotator::ZeroRotator;

    }

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







////////////////////////////  Ƽ ó  

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
            //    ⼭ UISubsystem         !
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateBossTimer(SpendedStage3Timer);
        }
    }
}

void ACooperationGameState::OnRep_UpdateTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), SpendedStage3Timer);
    UpdateTimer();
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACooperationGameState, CameraLocation);
    DOREPLIFETIME(ACooperationGameState, CameraRotation);
    DOREPLIFETIME(ACooperationGameState, CameraDistance);
    DOREPLIFETIME(ACooperationGameState, SpendedStage3Timer);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerCanMove);
    DOREPLIFETIME(ACooperationGameState, CurrentStageIndex);
    DOREPLIFETIME(ACooperationGameState, bIsPlayerBuffSelect);
    DOREPLIFETIME(ACooperationGameState, SelectBuffPlayer);
    DOREPLIFETIME(ACooperationGameState, Player1DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player2DataChanged);
    DOREPLIFETIME(ACooperationGameState, Player1StateData);
    DOREPLIFETIME(ACooperationGameState, Player2StateData);
    DOREPLIFETIME(ACooperationGameState, PlayerDataChanged);

    DOREPLIFETIME(ACooperationGameState, Enemy1StateData);
    DOREPLIFETIME(ACooperationGameState, Enemy2StateData);
    DOREPLIFETIME(ACooperationGameState, Enemy3StateData);
    DOREPLIFETIME(ACooperationGameState, Enemy4StateData);

    DOREPLIFETIME(ACooperationGameState, Enemy1DataChanged);
    DOREPLIFETIME(ACooperationGameState, Enemy2DataChanged);
    DOREPLIFETIME(ACooperationGameState, Enemy3DataChanged);
    DOREPLIFETIME(ACooperationGameState, Enemy4DataChanged);

    DOREPLIFETIME(ACooperationGameState, Player1ReceivedDamage);
    DOREPLIFETIME(ACooperationGameState, Player2ReceivedDamage);
    DOREPLIFETIME(ACooperationGameState, Player1DeathCount);
    DOREPLIFETIME(ACooperationGameState, Player2DeathCount);
    DOREPLIFETIME(ACooperationGameState, Player1ApplyAttackCount);
    DOREPLIFETIME(ACooperationGameState, Player2ApplyAttackCount);
    DOREPLIFETIME(ACooperationGameState, SpendedStage1Timer);
    DOREPLIFETIME(ACooperationGameState, SpendedStage2Timer);
    DOREPLIFETIME(ACooperationGameState, SpendedStage2Timer);
    DOREPLIFETIME(ACooperationGameState, bIsStage1Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage2Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Reached);
    DOREPLIFETIME(ACooperationGameState, bIsStage1Started);
    DOREPLIFETIME(ACooperationGameState, bIsStage2Started);
    DOREPLIFETIME(ACooperationGameState, bIsStage3Started);



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
            //    ⼭ UISubsystem         !
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
    ABaseWitch* DeadPlayer = Cast<ABaseWitch>(Player);
    if (DeadPlayer == CooperationGameGameMode->ActivePlayers[0])
    {
        Player1DeathCount++;
    }
    else if (DeadPlayer == CooperationGameGameMode->ActivePlayers[1])
    {
        Player2DeathCount++;
    }
}

void ACooperationGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
    Multicast_OnDeathMonster(Monster, DeathLocation);
}



void ACooperationGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
    //     Ŭ   ̾ Ʈ          
    if (IsValid(Attacker))
    {
        //   :  ǰ    ġ       Ʈ     
        // UGameplayStatics::SpawnEmitterAtLocation(...);
    }

    //             ó      ʿ  ϴٸ     ⼭      ( Ǵ                         )
}

void ACooperationGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{

}

void ACooperationGameState::Multicast_OnDeathPlayer_Implementation(ACharacter* Player, const FVector& DeathLocation)
{

}

void ACooperationGameState::Multicast_OnDeathMonster_Implementation(AActor* Monster, const FVector& DeathLocation)
{

}



void ACooperationGameState::InitEnemyInfo()
{
    if (HasAuthority())
    {
        EnemyInfos.Empty();
        FCharacterStateBuffer Enemy1State;
        FCharacterStateBuffer Enemy2State;
        FCharacterStateBuffer Enemy3State;
        FCharacterStateBuffer Enemy4State;

        EnemyInfos.Add(CooperationGameGameMode->ActiveEnemies[0], FPlayerData{
        "Enemy1", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Enemy1StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]];

        EnemyInfos.Add(CooperationGameGameMode->ActiveEnemies[1], FPlayerData{
        "Enemy2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Enemy2StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]];

        EnemyInfos.Add(CooperationGameGameMode->ActiveEnemies[2], FPlayerData{
        "Enemy3", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Enemy3StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]];

        EnemyInfos.Add(CooperationGameGameMode->ActiveEnemies[3], FPlayerData{
        "Enemy4", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Enemy4StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]];

        InitPlayerUIInfo();


        Cast<ABaseWitch>(CooperationGameGameMode->ActiveEnemies[0])->ResetCharacterState();
        Cast<ABaseWitch>(CooperationGameGameMode->ActiveEnemies[1])->ResetCharacterState();
        Cast<ABaseWitch>(CooperationGameGameMode->ActiveEnemies[1])->ResetCharacterState();
        Cast<ABaseWitch>(CooperationGameGameMode->ActiveEnemies[1])->ResetCharacterState();
    }
}

void ACooperationGameState::UpdateEnemyInfo(const FCharacterStateBuffer& State)
{

    UE_LOG(LogTemp, Warning, TEXT("Enemy Update Info"));

    if (State.OwnWitch == CooperationGameGameMode->ActiveEnemies[0])
    {
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].CurrentHP = State.CurrentHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].CurrentEXP = State.CurrentEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].CurrentMana = State.CurrentMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].MaxHP = State.MaxHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].MaxEXP = State.MaxEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].MaxMana = State.MaxMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].PlayerLevel = State.PlayerLevel;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].LifePoint = State.LifePoint;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]].AirbornePercent = State.AirbornePercent;
        Enemy1StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[0]];
        CooperationGameGameMode->RequestUpdateUI(2);
        UE_LOG(LogTemp, Warning, TEXT("1"));

    }

    if (State.OwnWitch == CooperationGameGameMode->ActiveEnemies[1])
    {
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].CurrentHP = State.CurrentHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].CurrentEXP = State.CurrentEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].CurrentMana = State.CurrentMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].MaxHP = State.MaxHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].MaxEXP = State.MaxEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].MaxMana = State.MaxMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].PlayerLevel = State.PlayerLevel;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].LifePoint = State.LifePoint;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]].AirbornePercent = State.AirbornePercent;
        Enemy1StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[1]];
        CooperationGameGameMode->RequestUpdateUI(2);
        UE_LOG(LogTemp, Warning, TEXT("2"));
    }

    if (State.OwnWitch == CooperationGameGameMode->ActiveEnemies[2])
    {
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].CurrentHP = State.CurrentHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].CurrentEXP = State.CurrentEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].CurrentMana = State.CurrentMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].MaxHP = State.MaxHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].MaxEXP = State.MaxEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].MaxMana = State.MaxMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].PlayerLevel = State.PlayerLevel;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].LifePoint = State.LifePoint;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]].AirbornePercent = State.AirbornePercent;
        Enemy1StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[2]];
        CooperationGameGameMode->RequestUpdateUI(2);
        UE_LOG(LogTemp, Warning, TEXT("3"));
    }

    if (State.OwnWitch == CooperationGameGameMode->ActiveEnemies[3])
    {
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].CurrentHP = State.CurrentHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].CurrentEXP = State.CurrentEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].CurrentMana = State.CurrentMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].MaxHP = State.MaxHP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].MaxEXP = State.MaxEXP;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].MaxMana = State.MaxMana;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].PlayerLevel = State.PlayerLevel;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].LifePoint = State.LifePoint;
        EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]].AirbornePercent = State.AirbornePercent;
        Enemy1StateData = EnemyInfos[CooperationGameGameMode->ActiveEnemies[3]];
        CooperationGameGameMode->RequestUpdateUI(2);
        UE_LOG(LogTemp, Warning, TEXT("4"));
    }
}


void ACooperationGameState::InitEnemyUIInfo()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {

            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->InitEnemyUI(&Enemy1StateData, &Enemy2StateData, &Enemy3StateData, &Enemy4StateData);
        }
    }
}

void ACooperationGameState::UpdateEnemyUIInfo()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            Cast<UCooperationWidget>(UISubsystem->CurrentActiveWidget)->UpdateEnemyUI(&Enemy1StateData, &Enemy2StateData, &Enemy3StateData, &Enemy4StateData);
        }
    }
}



void ACooperationGameState::OnRep_UpdateEnemy1DataUI()
{
    UpdateEnemyUIInfo();


}

void ACooperationGameState::OnRep_UpdateEnemy2DataUI()
{
    UpdateEnemyUIInfo();
}

void ACooperationGameState::OnRep_UpdateEnemy3DataUI()
{
    UpdateEnemyUIInfo();
}

void ACooperationGameState::OnRep_UpdateEnemy4DataUI()
{
    UpdateEnemyUIInfo();
}

void ACooperationGameState::OnRep_UpdateEnemyInitData()
{
    InitEnemyUIInfo();
}
