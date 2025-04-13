// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "../AddedWidget/MapSelectWidget.h"
#include "../AddedWidget/CharacterSelectWidget.h"
#include "../AddedWidget/GameSettingWidget.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MapSelectButton)
    {
        MapSelectButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMapSelect);
    }
        

    if (CharacterSelectButton)
    {
        CharacterSelectButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickCharacterSelect);
    }
        

    if (GameSettingButton)
    {
        GameSettingButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGameSetting);
    }
        

    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveDynamic(this, &ULobbyWidget::OnClickedMoveTitle);
        QuitButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedMoveTitle);
    }

    if (ReadyButton)
    {
        ReadyButton->OnClicked.RemoveDynamic(this, &ULobbyWidget::OnClickReady);
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickReady);
    }

    if (GameStartButton)
    {
        GameStartButton->OnClicked.RemoveDynamic(this, &ULobbyWidget::OnClickGameStart);
        GameStartButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGameStart);
    }

    // 기본으로 맵 선택만 보이게
    if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnClickMapSelect()
{
    if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Visible);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnClickCharacterSelect()
{
    if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Visible);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnClickGameSetting()
{
    if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Visible);
}



void ULobbyWidget::OnClickReady()
{
    //서버에 ready됐다고 알림
}


void ULobbyWidget::OnClickGameStart()
{
    //서버에 모두가 레디 상태라면 게임 시작하겠다고 알림
}