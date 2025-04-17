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

    /*if (MapSelectButton)
    {
        MapSelectButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMapSelect);
    }*/

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

    //MapSelectWidget->InitWidget(UIHandle);
    CharacterSelectWidget->InitWidget(UIHandle);
    GameSettingWidget->InitWidget(UIHandle);
}

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �⺻���� �� ���ø� ���̰�
    //if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);

    bIsReadied = false;

    if (CheckValidOfPlayerController())
    {
        if (OwningPC->HasAuthority())
        {
            GameStartButton->SetVisibility(ESlateVisibility::Visible);
            //MapSelectButton->SetVisibility(ESlateVisibility::Visible);
            ReadyButton->SetVisibility(ESlateVisibility::Visible);
            GameSettingButton->SetVisibility(ESlateVisibility::Visible);
            CharacterSelectButton->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            GameStartButton->SetVisibility(ESlateVisibility::Collapsed);
            //MapSelectButton->SetVisibility(ESlateVisibility::Collapsed);
            ReadyButton->SetVisibility(ESlateVisibility::Visible);
            GameSettingButton->SetVisibility(ESlateVisibility::Collapsed);
            CharacterSelectButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void ULobbyWidget::OnClickMapSelect()
{
    PlayUIEffectSound(EUISfxSoundType::Open);
    //if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Visible);
    if (CharacterSelectWidget) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (GameSettingWidget) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnClickCharacterSelect()
{
    //if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (IsValid(CharacterSelectWidget))
    {
        if (CharacterSelectWidget->GetVisibility() == ESlateVisibility::Visible)
        {
            PlayUIEffectSound(EUISfxSoundType::Close);
            CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            PlayUIEffectSound(EUISfxSoundType::Open);
            CharacterSelectWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if (IsValid(GameSettingWidget)) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::OnClickGameSetting()
{
    PlayUIEffectSound(EUISfxSoundType::Click);
    if (IsValid(GameSettingWidget))
    {
        if (GameSettingWidget->GetVisibility() == ESlateVisibility::Visible)
        {
            PlayUIEffectSound(EUISfxSoundType::Close);
            GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            PlayUIEffectSound(EUISfxSoundType::Open);
            GameSettingWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }

    //if (IsValid(GameSettingWidget)) GameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);

    //if (MapSelectWidget) MapSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (IsValid(CharacterSelectWidget)) CharacterSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void ULobbyWidget::OnClickReady()
{
    PlayUIEffectSound(EUISfxSoundType::Click);
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
    PlayUIEffectSound(EUISfxSoundType::Click);
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
