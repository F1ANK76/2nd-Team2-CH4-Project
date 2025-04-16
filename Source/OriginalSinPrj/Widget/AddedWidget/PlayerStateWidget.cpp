// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"


void UPlayerStateWidget::InitPlayerState(const FPlayerData& Data)
{
    PlayerNameText->SetText(FText::FromString(Data.PlayerName));
    CharacterImageUI->SetBrushFromTexture(Data.CharacterImage);

    HPBar->SetPercent(1.0f);
    ManaBar->SetPercent(Data.CurrentMana / Data.MaxMana);
    EXPBar->SetPercent(Data.CurrentEXP/ Data.MaxEXP);

    FString PercentString = FString::Printf(TEXT("%d%%"), Data.AirbornePercent);
    AirbornePercentText->SetText(FText::FromString(PercentString));
    PlayerLevel->SetText(FText::AsNumber(Data.PlayerLevel));
    PlayerLifes->SetText(FText::AsNumber(Data.LifePoint));
}


void UPlayerStateWidget::UpdateStatus(const FPlayerData& Data)
{
    // HP, Mana, Special ���� �ǽð� ��ġ�� ����
    HPBar->SetPercent(Data.CurrentHP / Data.MaxHP);
    ManaBar->SetPercent(Data.CurrentMana / Data.MaxMana);
    EXPBar->SetPercent(Data.CurrentEXP / Data.MaxEXP);

    FString PercentString = FString::Printf(TEXT("%d%%"), Data.AirbornePercent);
    AirbornePercentText->SetText(FText::FromString(PercentString));
    PlayerLevel->SetText(FText::AsNumber(Data.PlayerLevel));
    PlayerLifes->SetText(FText::AsNumber(Data.LifePoint));
}

