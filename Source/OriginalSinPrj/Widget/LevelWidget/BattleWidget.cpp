// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"

void UBattleWidget::InitPlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2)
{
    Player1StateUI->InitPlayerState(*Player1);
    Player2StateUI->InitPlayerState(*Player2);
}

//GameMode 혹은 Subsystem쪽에서 업데이트 해주면, UI의 값을 바꾸는 함수.
void UBattleWidget::UpdatePlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2)
{
    Player1StateUI->UpdateStatus(*Player1);
    Player2StateUI->UpdateStatus(*Player2);
}