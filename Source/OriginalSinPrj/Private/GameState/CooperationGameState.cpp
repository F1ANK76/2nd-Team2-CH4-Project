// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CooperationGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "GameMode/CooperationGameMode.h"
#include "Net/UnrealNetwork.h"
#include "../GameInstance/DataSubsystem.h"

ACooperationGameState::ACooperationGameState()
{
    PrimaryActorTick.bCanEverTick = false;
    bIsStage3Started = false;
    Timer = 0.0f;
    bReplicates = true;

    bPlayer1SelectedBuff = false;
    bPlayer2SelectedBuff = false;

}

void ACooperationGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // 게임 스테이트 클래스에서 게임 모드에 접근
    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
    CooperationGameGameMode = Cast<ACooperationGameMode>(GameModeBase);

    UE_LOG(LogTemp, Warning, TEXT("GameState BeginPlay"));
}
void ACooperationGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority() && bIsStage3Started)
    {
        Timer += DeltaSeconds;
    }
}



void ACooperationGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
    DOREPLIFETIME(ACooperationGameState, bPlayer1SelectedBuff);
    DOREPLIFETIME(ACooperationGameState, bPlayer2SelectedBuff);
    
}


//보스전 타이머 켜기
void ACooperationGameState::TurnOnTimer()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsStage3Started = true;
}

void ACooperationGameState::InitPlayerInfo()
{
    PlayerInfos.Empty(); // 기존 정보 삭제

    // 각 플레이어 정보 초기화
    /*
    플레이어의 정보를 받아오고 맵에 저장하기.
    */

}
void ACooperationGameState::UpdatePlayerInfo()
{
    // 각 플레이어 정보 업뎃
/*

*/
}
void ACooperationGameState::InitPlayerUIInfo()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //UISubSystem이나 gameInstance에게 UI 띄우라고 명령
            //Player에게 UI정보 바꾸라고 명령.
            //여기에 있는 PlayerInfo 활용해서...
        }
    }
}
void ACooperationGameState::UpdatePlayerUIInfo()
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

//플레이어에게 버프 선택 UI 열도록 시키기

void ACooperationGameState::RequestPlayerToOpenBuffUI()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            //Player에게 BuffSelect U열라고 명령
            //버프 종류는 여기서 결정.
            //버프 종류가 기록되어있는 목록을 받아서, 겹치지 않게 세개를 받아 선택지에 올리기
            /*
            TArray<FBuffType> Buff = {}; //  { 1번, 2번, ... , n번 };
            TArray<FBuffType> SelectedBuff;
            if (Buff.Num() > 0)
            {
                int RandomIndex = FMath::RandRange(0, Buff.Num() - 1);
            }

            for (int i = 0; i < 3; i++)
            {
                int32 Index = FMath::RandRange(0, Buff.Num() - 1);
                SelectedBuff.Add(Buff[Index]);
                Buff.RemoveAt(Index);
            }
            */
            //PlayerController?->UISubsystem->OpenWidget(EWidgetType::BuffSelectWidget);
            //PlayerController?->BuffSelectWidget->InitializeBuffs(SelectedBuff);
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

    if (player == PlayerControllerSet[0]) //<- 이게 맞나?
    {
        Player1Stage1SelectedBuff = Bufftype;

        bPlayer1SelectedBuff = true;
    }

    if (player == PlayerControllerSet[1])
    {
        Player2Stage1SelectedBuff = Bufftype;
        bPlayer2SelectedBuff = true;
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
            Player1Stage1SelectedBuff;
            Player2Stage1SelectedBuff;
            */
        }
        else if (CooperationGameGameMode->StageIndex == 3)
        {
            /*
            PlayerInfo += 
            Player1Stage2SelectedBuff;
            Player2Stage2SelectedBuff;
            */
        }
    }
    

    bPlayer1SelectedBuff = false;
    bPlayer2SelectedBuff = false;
}

void ACooperationGameState::AddExperienceToPlayer(APlayerController* Player, int32 Amount)
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

void ACooperationGameState::CheckLevelUp(APlayerController* Player)
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

//맵의 컨트롤러 등록해두기
void ACooperationGameState::RegisterInitialController(APlayerController* PC)
{
    if (IsValid(PC) && !PlayerControllerSet.Contains(PC))
    {
        PlayerControllerSet.Add(PC);
        UE_LOG(LogTemp, Log, TEXT("Controller Registered: %s"), *PC->GetName());
    }
}
