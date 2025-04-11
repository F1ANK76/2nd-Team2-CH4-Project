// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"


void UPlayerStateWidget::InitPlayerState(const FPlayerUIData& Data)
{
    PlayerNameText->SetText(FText::FromString(Data.PlayerName));
    CharacterImageUI->SetBrushFromTexture(Data.CharacterImage);

    HPBar->SetPercent(1.0f);
    ManaBar->SetPercent(Data.CurrentMana / Data.MaxMana);
    EXPBar->SetPercent(Data.CurrentEXP/ Data.MaxEXP);

    FString PercentString = FString::Printf(TEXT("%d%%"), Data.AirbornePercent);
    AirbornePercentText->SetText(FText::FromString(PercentString));
}


void UPlayerStateWidget::UpdateStatus(const FPlayerUIData& Data)
{
    // HP, Mana, Special 같은 실시간 수치만 갱신
    HPBar->SetPercent(Data.CurrentHP / Data.MaxHP);
    ManaBar->SetPercent(Data.CurrentMana / Data.MaxMana);
    EXPBar->SetPercent(Data.CurrentEXP / Data.MaxEXP);

    FString PercentString = FString::Printf(TEXT("%d%%"), Data.AirbornePercent);
    AirbornePercentText->SetText(FText::FromString(PercentString));
}
