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

//GameMode Ȥ�� Subsystem�ʿ��� ������Ʈ ���ָ�, UI�� ���� �ٲٴ� �Լ�.
void UBattleWidget::UpdatePlayerUI(FPlayerData Player1, FPlayerData Player2)
{
    Player1StateUI->UpdateStatus(Player1);
    Player2StateUI->UpdateStatus(Player2);
}



void UBattleWidget::UpdateFarmingModeTimerUI(float time)
{
    float RoundedTime = FMath::RoundToFloat(time * 100) / 100.f; // �Ҽ��� ��° �ڸ����� �ݿø�
    int32 Seconds = FMath::FloorToInt(RoundedTime);
    int32 Decimals = FMath::RoundToInt((RoundedTime - Seconds) * 100); // �Ҽ��� �Ʒ� ��° �ڸ�

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