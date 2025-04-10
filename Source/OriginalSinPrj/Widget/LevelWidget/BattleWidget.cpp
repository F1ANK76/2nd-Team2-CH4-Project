// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"

void UBattleWidget::InitPlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2)
{
    Player1StateUI->InitPlayerState(*Player1);
    Player2StateUI->InitPlayerState(*Player2);
}

//GameMode Ȥ�� Subsystem�ʿ��� ������Ʈ ���ָ�, UI�� ���� �ٲٴ� �Լ�.
void UBattleWidget::UpdatePlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2)
{
    Player1StateUI->UpdateStatus(*Player1);
    Player2StateUI->UpdateStatus(*Player2);
}