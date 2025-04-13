// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"
#include "Components/TextBlock.h"

void UBattleWidget::NativeConstruct()
{
    DeactiveFarmingModeWidget();
    DeactiveTimeLimitModeWidget();
}

void UBattleWidget::InitPlayerUI(FPlayerData Player1, FPlayerData Player2)
{
    Player1StateUI->InitPlayerState(Player1);
    Player2StateUI->InitPlayerState(Player2);
}

//GameMode 혹은 Subsystem쪽에서 업데이트 해주면, UI의 값을 바꾸는 함수.
void UBattleWidget::UpdatePlayerUI(FPlayerData Player1, FPlayerData Player2)
{
    Player1StateUI->UpdateStatus(Player1);
    Player2StateUI->UpdateStatus(Player2);
}



void UBattleWidget::UpdateFarmingModeTimerUI(float time)
{
    float RoundedTime = FMath::RoundToFloat(time * 100) / 100.f; // 소수점 둘째 자리까지 반올림
    int32 Seconds = FMath::FloorToInt(RoundedTime);
    int32 Decimals = FMath::RoundToInt((RoundedTime - Seconds) * 100); // 소수점 아래 둘째 자리

    FString TimeString = FString::Printf(TEXT("%d:%02d"), Seconds, Decimals);
    FarmingModeTimer->SetText(FText::FromString(TimeString));
}


void UBattleWidget::UpdateTimerLimitModeTimerUI(float time)
{

    TimeLimitModeTimer->SetText(FText::AsNumber(FMath::RoundToFloat(time)));
}


void UBattleWidget::ActiveFarmingModeWidget()
{
    FarmingModeTimer->SetVisibility(ESlateVisibility::Visible);
}

void UBattleWidget::ActiveTimeLimitModeWidget()
{
    TimeLimitModeTimer->SetVisibility(ESlateVisibility::Visible);
}

void UBattleWidget::DeactiveFarmingModeWidget()
{
    FarmingModeTimer->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleWidget::DeactiveTimeLimitModeWidget()
{
    TimeLimitModeTimer->SetVisibility(ESlateVisibility::Collapsed);
}