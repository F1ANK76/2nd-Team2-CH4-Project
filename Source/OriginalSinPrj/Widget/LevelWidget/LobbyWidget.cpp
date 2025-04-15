// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "../AddedWidget/MapSelectWidget.h"
#include "../AddedWidget/CharacterSelectWidget.h"
#include "../AddedWidget/GameSettingWidget.h"
#include "Player/Controller/WitchController.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"


void ULobbyWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);

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
        QuitButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedMoveTitle);
    }

    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickReady);
    }

    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGameStart);
    }

    MapSelectWidget->InitWidget(UIHandle);
    CharacterSelectWidget->InitWidget(UIHandle);
    GameSettingWidget->InitWidget(UIHandle);
}

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �⺻���� �� ���ø� ���̰�
    if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);

    bIsReadied = false;
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
    if (!CheckValidOfPlayerController())
    {
        return;
    }

    if (bIsReadied)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnClicked Unready"));
        OwningPC->OnUnreadiedClient();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnClicked Ready"));
        OwningPC->OnReadiedClient();
    }
    //������ ready�ƴٰ� �˸�
}


void ULobbyWidget::OnClickGameStart()
{
    if (!CheckValidOfPlayerController())
    {
        return;
    }
    //������ ��ΰ� ���� ���¶�� ���� �����ϰڴٰ� �˸�
    OwningPC->OnClickedStartButton();
}

bool ULobbyWidget::CheckValidOfGameInstance()
{
    if (!IsValid(GetGameInstance()))
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance is invalid"));
        return false;
    }

    LocalGameInstance = GetGameInstance<UOriginalSinPrjGameInstance>();

    if (!IsValid(LocalGameInstance))
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance Cast Fail"));
        return false;
    }

    return true;
}

bool ULobbyWidget::CheckValidOfPlayerController()
{
    if (!CheckValidOfGameInstance())
    {
        return false;
    }

    APlayerController* NewPC = LocalGameInstance->GetOwningPlayerController();

    if (!IsValid(NewPC))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Controller is invalid"));
        return false;
    }

    OwningPC = Cast<AWitchController>(NewPC);

    if (!IsValid(OwningPC))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Controller Cast Fail"));
        return false;
    }

    return true;
}
