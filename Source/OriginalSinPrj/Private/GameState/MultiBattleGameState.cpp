#include "GameState/MultiBattleGameState.h"
#include "GameMode/MultiBattleGameMode.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "Player/BaseWitch.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/Widget/AddedWidget/BuffSelectWidget.h"
#include "OriginalSinPrj/Widget/LevelWidget/BattleWidget.h"

AMultiBattleGameState::AMultiBattleGameState()
{
    bReplicates = true;
    bAlwaysRelevant = true;

    CameraLocation = FVector(-400.f, 200.f, 0.f);
    CameraRotation = FRotator::ZeroRotator;
    CameraDistance = 0;   
}

void AMultiBattleGameState::BeginPlay()
{
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    MultiBattleGameMode = Cast<AMultiBattleGameMode>(GameModeBase);

    GameInstance = Cast<UOriginalSinPrjGameInstance>(GetGameInstance());

    InitCharacterSounds();
}

void AMultiBattleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMultiBattleGameState, CameraLocation);
    DOREPLIFETIME(AMultiBattleGameState, CameraRotation);
    DOREPLIFETIME(AMultiBattleGameState, CameraDistance);
    DOREPLIFETIME(AMultiBattleGameState, Player1DataChanged);
    DOREPLIFETIME(AMultiBattleGameState, Player2DataChanged);
    DOREPLIFETIME(AMultiBattleGameState, Player1StateData);
    DOREPLIFETIME(AMultiBattleGameState, Player2StateData);
    DOREPLIFETIME(AMultiBattleGameState, PlayerDataChanged);
    DOREPLIFETIME(AMultiBattleGameState, bIsPlayerBuffSelect);
    DOREPLIFETIME(AMultiBattleGameState, SelectBuffPlayer);
    DOREPLIFETIME(AMultiBattleGameState, bIsPlayerCanMove); 
    
    DOREPLIFETIME(AMultiBattleGameState, Player1ReceivedDamage);
    DOREPLIFETIME(AMultiBattleGameState, Player2ReceivedDamage);
    DOREPLIFETIME(AMultiBattleGameState, Player1DeathCount);
    DOREPLIFETIME(AMultiBattleGameState, Player2DeathCount);
    DOREPLIFETIME(AMultiBattleGameState, Player1ApplyAttackCount);
    DOREPLIFETIME(AMultiBattleGameState, Player2ApplyAttackCount);
}

void AMultiBattleGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SetCameraTransform();
}

void AMultiBattleGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
}

void AMultiBattleGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
}

void AMultiBattleGameState::FinishMatch()
{
}

void AMultiBattleGameState::VictoryMatch()
{
}

void AMultiBattleGameState::DefeatMatch()
{
}

void AMultiBattleGameState::DrawMatch()
{
}

void AMultiBattleGameState::InitPlayerInfo()
{
    if (HasAuthority())  // ���������� ����Ǵ� �ڵ�
    {
        PlayerInfos.Empty(); // ���� ���� ����
        //��𼱰� �޾ƿ;���...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(FPlayerData{
        "Player1", nullptr, 
        100.0f, 100.0f, 
        0, 0, 
        0, 0.0f, 
        0, 
        0, 
        0, 100, 3 });
        Player1StateData = PlayerInfos[0];
        
        PlayerInfos.Add(FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 3 });
        Player2StateData = PlayerInfos[1];
        InitPlayerUIInfo();
        
        Cast<ABaseWitch>(MultiBattleGameMode->ActivePlayers[0])->ResetCharacterState();
        Cast<ABaseWitch>(MultiBattleGameMode->ActivePlayers[1])->ResetCharacterState();
    }
}

void AMultiBattleGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{
    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    
    if (State.OwnWitch == MultiBattleGameMode->ActivePlayers[0])
    {
        PlayerInfos[0].CurrentEXP = State.CurrentEXP;
        
        if (PlayerInfos[0].CurrentMana != State.CurrentMana)
        {
            Player1ApplyAttackCount++;
            PlayerInfos[0].CurrentMana = State.CurrentMana;
        }
        
        PlayerInfos[0].MaxHP = State.MaxHP;
        PlayerInfos[0].MaxEXP = State.MaxEXP;
        PlayerInfos[0].MaxMana = State.MaxMana;
        PlayerInfos[0].PlayerLevel = State.PlayerLevel;
        PlayerInfos[0].LifePoint = State.LifePoint;
        PlayerInfos[0].AirbornePercent = State.AirbornePercent;
        Player1StateData = PlayerInfos[0];
        MultiBattleGameMode->RequestUpdateUI(0);
        
    }
    
    
    
    if (State.OwnWitch == MultiBattleGameMode->ActivePlayers[1])
    {
        PlayerInfos[1].CurrentEXP = State.CurrentEXP;
        
        if (PlayerInfos[1].CurrentMana != State.CurrentMana)
        {
            Player2ApplyAttackCount++;
            PlayerInfos[1].CurrentMana = State.CurrentMana;
        }

        PlayerInfos[1].MaxHP = State.MaxHP;
        PlayerInfos[1].MaxEXP = State.MaxEXP;
        PlayerInfos[1].MaxMana = State.MaxMana;
        PlayerInfos[1].PlayerLevel = State.PlayerLevel;
        PlayerInfos[1].LifePoint = State.LifePoint;
        PlayerInfos[1].AirbornePercent = State.AirbornePercent;
        Player2StateData = PlayerInfos[1];
        MultiBattleGameMode->RequestUpdateUI(1);
    }
}

void AMultiBattleGameState::SetCameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;

    for (AActor* Player : MultiBattleGameMode->AlivePlayers)
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
    if (MultiBattleGameMode->AlivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / MultiBattleGameMode->AlivePlayers.Num();
    }

    if (HasAuthority()) // �������� Ȯ��
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);

        CameraLocation = MeanPlayerLocation;
        CameraRotation = FRotator::ZeroRotator;
    }
    float CamDist = CameraDistance + 200.0f;
    FRotator CamRot = CameraRotation;
    FVector CamLoc = CameraLocation + FVector(FMath::Min(-CamDist, -500.0f) / 2.0f, 0.0f, 0.0f);
    
    MultiBattleGameMode->SpawnedBaseCamera[0]->CameraComponent->SetWorldLocation(
        FMath::VInterpTo(MultiBattleGameMode->SpawnedBaseCamera[0]->CameraComponent->GetComponentLocation(),
            CamLoc,
            0.01f,
            5.0f));
    MultiBattleGameMode->SpawnedBaseCamera[0]->CameraComponent->SetWorldRotation(CamRot);
    //MultiBattleGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}

void AMultiBattleGameState::InitPlayerUIInfo()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void AMultiBattleGameState::UpdatePlayerUIInfo()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // ���⼭ UISubsystem ��� ����!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->UpdatePlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void AMultiBattleGameState::RegisterInitialController(APlayerController* PC)
{
    if (IsValid(PC) && !PlayerControllerSet.Contains(PC))
    {
        PlayerControllerSet.Add(PC);
        UE_LOG(LogTemp, Log, TEXT("Controller Registered: %s"), *PC->GetName());
    }
}

void AMultiBattleGameState::TurnOnBattleWidget()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (IsValid(MyGI))
        {
            UE_LOG(LogTemp, Warning, TEXT("GameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is ReadyGameGI Is Ready"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("InValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValidInValid"));
        }
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            if (IsValid(UISubsystem->CurrentActiveWidget))
            {
                UE_LOG(LogTemp, Warning, TEXT("IValidlilasidladilasiladd"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("INVALID"));
            }
            UE_LOG(LogTemp, Warning, TEXT("I::::::::::::%d "), UISubsystem->GetCurrentLevelType());
            
            // ���⼭ UISubsystem ��� ����!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActivePlayerWidget();
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActiveEnemyWidget();
        }
    }
}

void AMultiBattleGameState::SetPlayerMove(bool bCanMove)
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

void AMultiBattleGameState::ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype)
{
    if (IsValid(MultiBattleGameMode))
    {
        
    }

    if (player == PlayerControllerSet[0]) //<- �̰� �³�?
    {
        Player1SelectedBuff = Bufftype;

    }

    if (player == PlayerControllerSet[1])
    {
        Player2SelectedBuff = Bufftype;
    }

    
}

void AMultiBattleGameState::ApplyBuffStat()
{
    if (IsValid(MultiBattleGameMode))
    {
        /*
            PlayerInfo += 
            SelectedBuff[2];
            SelectedBuff[3];
            */
    }
    //UI ���� ���

    bIsPlayerBuffSelect = 0;
}

void AMultiBattleGameState::CreateBuffSelectUI(AWitchController* Controller)
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

void AMultiBattleGameState::CloseBuffSelectUI()
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

TArray<EBuffType> AMultiBattleGameState::BuffUIInit()
{
    //���� ������ ���⼭ ����.
    //���� ������ ��ϵǾ��ִ� ����� �޾Ƽ�, ��ġ�� �ʰ� ������ �޾� �������� �ø���

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


void AMultiBattleGameState::OnRep_UpdatePlayerInitData()
{
    InitPlayerUIInfo();
}

void AMultiBattleGameState::OnRep_UpdatePlayer1DataUI()
{
    UpdatePlayerUIInfo();
}

void AMultiBattleGameState::OnRep_UpdatePlayer2DataUI()
{
    UpdatePlayerUIInfo();
}

void AMultiBattleGameState::OnRep_TurnOffBuffUI()
{
    CloseBuffSelectUI();
}

void AMultiBattleGameState::OnRep_SetPlayerMove()
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

void AMultiBattleGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::Multicast_PlayAttackPlayerAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayBeAttackedPlayerAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayAttackMonsterAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayBeAttackedMonsterAnimation_Implementation(AActor* Attacker)
{
}