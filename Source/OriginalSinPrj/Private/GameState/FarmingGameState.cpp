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
    // 초기값 (예: 1분)
    TimeRemaining = 60.0f;
    bReplicates = true;

    //싱글 / 멀티 분기 나누기
    //싱글 -> 포스트
    //멀티에서는 이미 로그인이라 포스트 로그인 호출 불가능 -> 처리 필요 -> 포스트 심리스



}

void AFarmingGameState::ShowUI(APlayerController* PlayerController)
    {
        if (PlayerController && PlayerController->IsLocalController()) // 로컬 컨트롤러인지 확인
        {
            UBattleWidget* Widget = CreateWidget<UBattleWidget>(PlayerController, BattleWidgetClass);
            if (Widget)
            {
                Widget->AddToViewport(); // 위젯을 화면에 띄우기
                Widget->ActiveFarmingModeWidget(); // UI 활성화
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
            ShowUI(PlayerController); // UI 띄우는 함수 호출
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
    // 다음 라운드 시작 or 결과 화면 전환 등 처리
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
        // 커스텀 PlayerController에서 위젯을 찾는 방법
        // 예를 들어, PlayerController가 'APlayerController' 클래스에서 상속된 클래스일 경우
        APlayerController* CustomPC = Cast<APlayerController>(PlayerController);
        if (CustomPC)
        {
            //return CustomPC->GetCurrentBattleWidget(); // 위젯을 반환하는 함수 호출
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
    // UI 업데이트
    if (Player->IsLocalController())
    {
        if (UBattleWidget* Widget = GetBattleWidgetFromController(Player))
        {
            FPlayerUIData UIData;
            UIData.Experience = Info.Experience;
            UIData.Level = Info.Level;

            // 두 명의 플레이어 UI가 있을 경우, 구분해서 전달 (여기선 단일 예시로)
            Widget->UpdatePlayerUI(UIData, UIData);
        }
    }
    */
}




APlayerController* AFarmingGameState::GetPlayerController() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    // 일반적으로 로컬 플레이어는 0번 인덱스
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
