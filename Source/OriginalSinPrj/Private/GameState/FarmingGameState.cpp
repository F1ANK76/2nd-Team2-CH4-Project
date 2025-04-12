#include "GameState/FarmingGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/FarmingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Player/BaseWitch.h"

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
            if (TimeRemaining <= 0.0f)
            {
                EndFarmingMode();
            }
        }
    }
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
            UpdatePlayerUIInfo(); // UI 갱신
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
    PlayerInfos.Empty(); // 기존 정보 삭제

    // 각 플레이어 정보 초기화
    /*
    플레이어의 정보를 받아오고 맵에 저장하기.
    */
}

// 게임 시작 시, 플레이어 정보를 초기화하는 함수
void AFarmingGameState::UpdatePlayerInfo()
{

    // 각 플레이어 정보 초기화
    /*
    
    */
}

void AFarmingGameState::InitPlayerUIInfo()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 UI정보 바꾸라고 명령.
            //여기에 있는 PlayerInfo 활용해서...
        }
    }
}

void AFarmingGameState::UpdatePlayerUIInfo()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 UI정보 바꾸라고 명령.
            //여기에 있는 PlayerInfo 활용해서...
        }
    }
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
}
