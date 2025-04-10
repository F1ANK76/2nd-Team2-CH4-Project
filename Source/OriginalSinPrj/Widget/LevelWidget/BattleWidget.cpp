// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"
#include "Components/TextBlock.h"

void UBattleWidget::NativeConstruct()
{
    
}

void UBattleWidget::InitPlayerUI(FPlayerUIData& Player1, FPlayerUIData& Player2)
{
    Player1StateUI->InitPlayerState(Player1);
    Player2StateUI->InitPlayerState(Player2);
}

//GameMode Ȥ�� Subsystem�ʿ��� ������Ʈ ���ָ�, UI�� ���� �ٲٴ� �Լ�.
void UBattleWidget::UpdatePlayerUI(FPlayerUIData& Player1, FPlayerUIData& Player2)
{
    Player1StateUI->UpdateStatus(Player1);
    Player2StateUI->UpdateStatus(Player2);
}



void UBattleWidget::UpdateFarmingModeTimerUI(float time)
{
    FarmingModeTimer->SetText(FText::AsNumber(FMath::RoundToFloat(time)));
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