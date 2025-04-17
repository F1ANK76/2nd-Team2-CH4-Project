#include "GameState/FarmingGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/FarmingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Player/BaseWitch.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"

AFarmingGameState::AFarmingGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsFarmingStarted = false;
    TimeRemaining = 60.0f;
    bReplicates = true;
}

// ShowUI는 서버에서 호출할 필요 없으므로 삭제 예정
void AFarmingGameState::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));

    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    FarmingGameMode = Cast<AFarmingGameMode>(GameModeBase);
}

void AFarmingGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority() && bIsFarmingStarted)
    {
        if (TimeRemaining > 0.0f)
        {
            TimeRemaining -= DeltaSeconds;
            TimeRemaining = FMath::Max(TimeRemaining, 0.0f);
            UpdateTimer();
            if (TimeRemaining <= 0.0f)
            {
                EndFarmingMode();
            }
            SetCameraTransform();
        }
    }
}




void AFarmingGameState::SetCameraTransform()
{
    FVector SumPlayerLocation = FVector::ZeroVector;
    float minY = 99999.f;
    float minZ = 99999.f;

    float maxY = -99999.f;
    float maxZ = -99999.f;


    for (AActor* Player : FarmingGameMode->AlivePlayers)
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
    if (FarmingGameMode->AlivePlayers.Num() > 0)
    {
        MeanPlayerLocation = SumPlayerLocation / FarmingGameMode->AlivePlayers.Num();
    }

    if (HasAuthority()) // 서버인지 확인
    {
        CameraDistance = maxY - minY + 2 * (maxZ - minZ);

        CameraLocation = MeanPlayerLocation;
        CameraRotation = FRotator::ZeroRotator;
    }

    FarmingGameMode->SpawnedBaseCamera[0]->UpdateCameraLocationandRotation();
}



void AFarmingGameState::StartFarmingMode()
{
    if (HasAuthority())
    {
        bIsFarmingStarted = true;


        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PlayerController = Cast<APlayerController>(*It);
            if (PlayerController)
            {
                if (PlayerController && !PlayerInfos.Contains(PlayerController))
                {
                    FPlayerData InitData; ///어디서든 받아오기
                    PlayerInfos.Add(PlayerController, InitData);
                }
            }
        }

        InitPlayerInfo();

        UE_LOG(LogTemp, Warning, TEXT("Farming Mode Started!"));
    }
}

void AFarmingGameState::EndFarmingMode()
{
    bIsFarmingStarted = false;
    UE_LOG(LogTemp, Warning, TEXT("Farming Mode Ended"));

    //alert to Gamemode
    FarmingGameMode->EndGame();
}



void AFarmingGameState::TurnOnPlayerUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActivePlayerWidget();
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActiveFarmingModeWidget();
        }
    }
}

void AFarmingGameState::TurnOnAllUI()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActivePlayerWidget();
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActiveEnemyWidget();
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActiveFarmingModeWidget();
        }
    }
}

void AFarmingGameState::OnRep_TurnOnAllUI()
{
    TurnOnAllUI();
}

// 몬스터 죽을 때 경험치를 추가하는 함수
void AFarmingGameState::AddExperienceToPlayer(APlayerController* Player, int32 Amount)
{
    if (Player)
    {
        // PlayerInfos에서 플레이어 정보 찾기
        FPlayerData* PlayerData = PlayerInfos.Find(Player);

        if (PlayerData)
        {
            PlayerData->CurrentEXP += Amount;
            CheckLevelUp(Player);
            //UpdatePlayerInfo(); // UI 갱신
        }
    }
}

void AFarmingGameState::CheckLevelUp(APlayerController* Player)
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



// 게임 시작 시, 플레이어 정보를 초기화하는 함수
void AFarmingGameState::InitPlayerInfo()
{
    if (HasAuthority())  // 서버에서만 실행되는 코드
    {
        PlayerInfos.Empty(); // 기존 정보 삭제
        //어디선가 받아와야해...
        FCharacterStateBuffer Player1State;
        FCharacterStateBuffer Player2State;

        PlayerInfos.Add(FarmingGameMode->ActivePlayers[0], FPlayerData{
        "Player1", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player1StateData = PlayerInfos[FarmingGameMode->ActivePlayers[0]];

        PlayerInfos.Add(FarmingGameMode->ActivePlayers[1], FPlayerData{
        "Player2", nullptr,
        100.0f, 100.0f,
        0, 0,
        0, 0.0f,
        0,
        0,
        0, 100, 1 });
        Player2StateData = PlayerInfos[FarmingGameMode->ActivePlayers[1]];
        InitPlayerUIInfo();

        Cast<ABaseWitch>(FarmingGameMode->ActivePlayers[0])->ResetCharacterState();
        Cast<ABaseWitch>(FarmingGameMode->ActivePlayers[1])->ResetCharacterState();
    }
}

// 게임 시작 시, 플레이어 정보를 초기화하는 함수
void AFarmingGameState::UpdatePlayerInfo(const FCharacterStateBuffer& State)
{
    UE_LOG(LogTemp, Warning, TEXT("Update Info"));
    if (FarmingGameMode->NetMode == ENetMode::NM_Standalone)
    {
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxHP = State.MaxHP;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxEXP = State.MaxEXP;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxMana = State.MaxMana;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].PlayerLevel = State.PlayerLevel;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].LifePoint = State.LifePoint;
        PlayerInfos[FarmingGameMode->ActivePlayers[0]].AirbornePercent = State.AirbornePercent;
        Player1StateData = PlayerInfos[FarmingGameMode->ActivePlayers[0]];
        FarmingGameMode->RequestUpdateUI(0);

    }
    else
    {
        if (State.OwnWitch == FarmingGameMode->ActivePlayers[0])
        {
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentHP = State.CurrentHP;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentEXP = State.CurrentEXP;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].CurrentMana = State.CurrentMana;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxHP = State.MaxHP;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxEXP = State.MaxEXP;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].MaxMana = State.MaxMana;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].PlayerLevel = State.PlayerLevel;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].LifePoint = State.LifePoint;
            PlayerInfos[FarmingGameMode->ActivePlayers[0]].AirbornePercent = State.AirbornePercent;
            Player1StateData = PlayerInfos[FarmingGameMode->ActivePlayers[0]];
            FarmingGameMode->RequestUpdateUI(0);

        }



        if (State.OwnWitch == FarmingGameMode->ActivePlayers[1])
        {
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].CurrentHP = State.CurrentHP;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].CurrentEXP = State.CurrentEXP;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].CurrentMana = State.CurrentMana;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].MaxHP = State.MaxHP;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].MaxEXP = State.MaxEXP;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].MaxMana = State.MaxMana;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].PlayerLevel = State.PlayerLevel;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].LifePoint = State.LifePoint;
            PlayerInfos[FarmingGameMode->ActivePlayers[1]].AirbornePercent = State.AirbornePercent;
            
            Player2StateData = PlayerInfos[FarmingGameMode->ActivePlayers[1]];
            FarmingGameMode->RequestUpdateUI(1);
        }
    }

}

////////////////////////////멀티 처리

void AFarmingGameState::OnRep_UpdatePlayer1DataUI()
{
    UpdatePlayerUIInfo();
}

void AFarmingGameState::OnRep_UpdatePlayer2DataUI()
{
    UpdatePlayerUIInfo();
}

void AFarmingGameState::InitPlayerUIInfo()
{
    //위젯 접근해서 갱신
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->InitPlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}

void AFarmingGameState::UpdatePlayerUIInfo()
{
    //위젯 접근해서 갱신
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->UpdatePlayerUI(&Player1StateData, &Player2StateData);
        }
    }
}


void AFarmingGameState::UpdateTimer()
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            // 여기서 UISubsystem 사용 가능!
            Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->UpdateFarmingModeTimerUI(TimeRemaining);
        }
    }
}

void AFarmingGameState::OnRep_UpdateTimer()
{
    UpdateTimer();
}



void AFarmingGameState::SetPlayerPawn(ABaseWitch* InPawn)
{
    PlayerPawnRef = InPawn;
}


void AFarmingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFarmingGameState, bIsFarmingStarted); 
    DOREPLIFETIME(AFarmingGameState, TimeRemaining);
    DOREPLIFETIME(AFarmingGameState, MultiPlayer);
    DOREPLIFETIME(AFarmingGameState, CameraLocation);
    DOREPLIFETIME(AFarmingGameState, CameraRotation);
    DOREPLIFETIME(AFarmingGameState, CameraDistance);
    DOREPLIFETIME(AFarmingGameState, Player1StateData);
    DOREPLIFETIME(AFarmingGameState, Player2StateData);
    DOREPLIFETIME(AFarmingGameState, Player1DataChanged);
    DOREPLIFETIME(AFarmingGameState, Player2DataChanged);
}
