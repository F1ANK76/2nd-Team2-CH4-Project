#include "GameState/MultiBattleGameState.h"
#include "GameMode/MultiBattleGameMode.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "Player/BaseWitch.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/Widget/LevelWidget/BattleWidget.h"

AMultiBattleGameState::AMultiBattleGameState()
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

void AMultiBattleGameState::BeginPlay()
{
    if (AMultiBattleGameMode* GM = Cast<AMultiBattleGameMode>(GetWorld()->GetAuthGameMode()))
    {
        MultiBattleGameMode = GM;
    }
}

void AMultiBattleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMultiBattleGameState, Player1StateData);
    DOREPLIFETIME(AMultiBattleGameState, Player2StateData);
    DOREPLIFETIME(AMultiBattleGameState, Player1DataChanged);
    DOREPLIFETIME(AMultiBattleGameState, Player2DataChanged);
    DOREPLIFETIME(AMultiBattleGameState, CameraLocation);
    DOREPLIFETIME(AMultiBattleGameState, CameraRotation);
    DOREPLIFETIME(AMultiBattleGameState, CameraDistance);
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

        PlayerInfos.Add(MultiBattleGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr, 
        100.0f, 100.0f, 
        0, 0, 
        0, 0.0f, 
        0, 
        0, 
        0, 100 });
        Player1StateData = PlayerInfos[MultiBattleGameMode->ActivePlayers[0]];
        PlayerInfos.Add(MultiBattleGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100 });
        Player2StateData = PlayerInfos[MultiBattleGameMode->ActivePlayers[1]];
    }
    InitPlayerUIInfo();

}

void AMultiBattleGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{
    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    
    if (State.OwnWitch == MultiBattleGameMode->ActivePlayers[0])
    {
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].MaxHP = State.MaxHP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].MaxEXP = State.MaxEXP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].MaxMana = State.MaxMana;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].LifePoint = State.LifePoint;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[0]].AirbornePercent = State.AirbornePercent;
        Player1StateData = PlayerInfos[MultiBattleGameMode->ActivePlayers[0]];
        MultiBattleGameMode->RequestUpdateUI(0);
    }
    
    if (State.OwnWitch == MultiBattleGameMode->ActivePlayers[1])
    {
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].MaxHP = State.MaxHP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].MaxEXP = State.MaxEXP;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].MaxMana = State.MaxMana;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].LifePoint = State.LifePoint;
        PlayerInfos[MultiBattleGameMode->ActivePlayers[1]].AirbornePercent = State.AirbornePercent;
        Player2StateData = PlayerInfos[MultiBattleGameMode->ActivePlayers[1]];
        MultiBattleGameMode->RequestUpdateUI(1);
    }
}

void AMultiBattleGameState::SetCameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    FVector SumEnemyLocation = FVector::ZeroVector;

    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;

    for (AActor* Player : MultiBattleGameMode->ActivePlayers)
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

    FVector MeanActorLocation = FVector::ZeroVector;

    int32 NumOfActors = MultiBattleGameMode->ActivePlayers.Num();

    if (MultiBattleGameMode->ActivePlayers.Num() > 0)
    {
        MeanActorLocation = (SumPlayerLocation + SumEnemyLocation) / NumOfActors;
    }


    if (HasAuthority())
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);
        CameraLocation = MeanActorLocation;
        CameraRotation = FRotator::ZeroRotator;

    }// �������� Ȯ��

    MultiBattleGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
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