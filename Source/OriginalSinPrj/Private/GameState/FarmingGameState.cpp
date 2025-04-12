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

// ShowUI�� �������� ȣ���� �ʿ� �����Ƿ� ���� ����
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
                    FPlayerData InitData; ///��𼭵� �޾ƿ���
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



// ���� ���� �� ����ġ�� �߰��ϴ� �Լ�
void AFarmingGameState::AddExperienceToPlayer(APlayerController* Player, int32 Amount)
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




// ���� ���� ��, �÷��̾� ������ �ʱ�ȭ�ϴ� �Լ�
void AFarmingGameState::InitPlayerInfo()
{
    PlayerInfos.Empty(); // ���� ���� ����

    // �� �÷��̾� ���� �ʱ�ȭ
    /*
    �÷��̾��� ������ �޾ƿ��� �ʿ� �����ϱ�.
    */
}

// ���� ���� ��, �÷��̾� ������ �ʱ�ȭ�ϴ� �Լ�
void AFarmingGameState::UpdatePlayerInfo()
{

    // �� �÷��̾� ���� �ʱ�ȭ
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
            //Player���� UI���� �ٲٶ�� ���.
            //���⿡ �ִ� PlayerInfo Ȱ���ؼ�...
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
            //Player���� UI���� �ٲٶ�� ���.
            //���⿡ �ִ� PlayerInfo Ȱ���ؼ�...
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
