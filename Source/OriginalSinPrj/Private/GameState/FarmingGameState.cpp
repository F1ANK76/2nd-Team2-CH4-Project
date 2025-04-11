// Fill out your copyright notice in the Description page of Project Settings.


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
    // �ʱⰪ (��: 1��)
    TimeRemaining = 60.0f;
    bReplicates = true;

    //�̱� / ��Ƽ �б� ������
    //�̱� -> ����Ʈ
    //��Ƽ������ �̹� �α����̶� ����Ʈ �α��� ȣ�� �Ұ��� -> ó�� �ʿ� -> ����Ʈ �ɸ���



}

void AFarmingGameState::ShowUI(APlayerController* PlayerController)
    {
        if (PlayerController && PlayerController->IsLocalController()) // ���� ��Ʈ�ѷ����� Ȯ��
        {
            UBattleWidget* Widget = CreateWidget<UBattleWidget>(PlayerController, BattleWidgetClass);
            if (Widget)
            {
                Widget->AddToViewport(); // ������ ȭ�鿡 ����
                Widget->ActiveFarmingModeWidget(); // UI Ȱ��ȭ
            }
        }
    }

void AFarmingGameState::BeginPlay()
{
    Super::BeginPlay();
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*It);
        if (PlayerController)
        {
            ShowUI(PlayerController); // UI ���� �Լ� ȣ��
        }
    }

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
            UE_LOG(LogTemp, Warning, TEXT("TimeRemaining: %f"), TimeRemaining);
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
        UE_LOG(LogTemp, Warning, TEXT("Farming Mode Started!"));
    }
}



void AFarmingGameState::EndFarmingMode()
{
    // ���� ���� ���� or ��� ȭ�� ��ȯ �� ó��
    UE_LOG(LogTemp, Warning, TEXT("FarmingMode Ended "));
}


void AFarmingGameState::AddExperienceToPlayer(APlayerController* Player, int32 Amount)
{
    /*
    if (!PlayerInfos.Contains(Player))
    {
        PlayerInfos.Add(Player, FPlayerFarmingInfo());
    }

    FPlayerFarmingInfo& Info = PlayerInfos[Player];
    Info.Experience += Amount;
    CheckLevelUp(Player);\
    */
}


void AFarmingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AFarmingGameState, bIsFarmingStarted);
    DOREPLIFETIME(AFarmingGameState, TimeRemaining);
    DOREPLIFETIME(AFarmingGameState, PlayerInfos);
}



UBattleWidget* AFarmingGameState::GetBattleWidgetFromController(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        // Ŀ���� PlayerController���� ������ ã�� ���
        // ���� ���, PlayerController�� 'APlayerController' Ŭ�������� ��ӵ� Ŭ������ ���
        APlayerController* CustomPC = Cast<APlayerController>(PlayerController);
        if (CustomPC)
        {
            //return CustomPC->GetCurrentBattleWidget(); // ������ ��ȯ�ϴ� �Լ� ȣ��
        }
    }
    return nullptr;
}




void AFarmingGameState::CheckLevelUp(APlayerController* Player)
{
    //FPlayerFarmingInfo& Info = PlayerInfos[Player];
    /*
    while (Info.Experience >= 100)
    {
        Info.Experience -= 100;
        Info.Level++;
    }
    */
    /*
    // UI ������Ʈ
    if (Player->IsLocalController())
    {
        if (UBattleWidget* Widget = GetBattleWidgetFromController(Player))
        {
            FPlayerUIData UIData;
            UIData.Experience = Info.Experience;
            UIData.Level = Info.Level;

            // �� ���� �÷��̾� UI�� ���� ���, �����ؼ� ���� (���⼱ ���� ���÷�)
            Widget->UpdatePlayerUI(UIData, UIData);
        }
    }
    */
}




APlayerController* AFarmingGameState::GetPlayerController() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    // �Ϲ������� ���� �÷��̾�� 0�� �ε���
    return UGameplayStatics::GetPlayerController(World, 0);
}

void AFarmingGameState::SetPlayerPawn(ABaseWitch* InPawn)
{
    PlayerPawnRef = InPawn;
}

ABaseWitch* AFarmingGameState::GetPlayerPawn() const
{
    return PlayerPawnRef;
}
