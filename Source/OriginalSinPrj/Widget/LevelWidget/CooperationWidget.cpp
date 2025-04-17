// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperationWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"


void UCooperationWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);


    DeactivateAllWidgets();
}

void UCooperationWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ProgressBarColors = {
    FLinearColor(0.47f, 0.25f, 0.31f, 1.f),
    FLinearColor(0.32f, 0.29f, 0.47f, 1.f),
    FLinearColor(0.14f, 0.14f, 0.48f, 1.f),
    FLinearColor(0.23f, 0.45f, 0.48f, 1.f),
    FLinearColor(0.23f, 0.48f, 0.25f, 1.f),
    FLinearColor(0.74f, 0.68f, 0.25f, 1.f),
    FLinearColor(0.74f, 0.26f, 0.71f, 1.f),
    FLinearColor(0.f, 0.f, 0.f, 0.37f)
    };    
}



void UCooperationWidget::ActiveWidgetbyStage(int32 StageIndex)
{
    switch (StageIndex)
    {
    case 1:
        ActiveStage1Widget();
        break;

    case 2:
        ActiveStage1Widget();
        ActiveStage2Widget();
        break;

    case 3:
        ActiveStage1Widget();
        ActiveStage3Widget();
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid StageIndex: %d"), StageIndex);
        break;
    }
}

void UCooperationWidget::ActiveStage1Widget()
{
    DeactivateAllWidgets();
    Player1StateUI->SetVisibility(ESlateVisibility::Visible);
    Player2StateUI->SetVisibility(ESlateVisibility::Visible);

}
void UCooperationWidget::ActiveStage2Widget()
{
    ActiveStage1Widget();
    Enemy1StateUI->SetVisibility(ESlateVisibility::Visible);
    Enemy2StateUI->SetVisibility(ESlateVisibility::Visible);
    Enemy3StateUI->SetVisibility(ESlateVisibility::Visible);
    Enemy4StateUI->SetVisibility(ESlateVisibility::Visible);
}
void UCooperationWidget::ActiveStage3Widget()
{
    ActiveStage1Widget();
    BossHPBar->SetVisibility(ESlateVisibility::Visible);
    BossCurrentHPText->SetVisibility(ESlateVisibility::Visible);
    BossMaxHPText->SetVisibility(ESlateVisibility::Visible);
    BossNameText->SetVisibility(ESlateVisibility::Visible);
    RemainHPLine->SetVisibility(ESlateVisibility::Visible);
    ShowForwardSlash->SetVisibility(ESlateVisibility::Visible);
    BossModeTimer->SetVisibility(ESlateVisibility::Visible);
}


void UCooperationWidget::DeactivateAllWidgets()
{
    
    // Player �ܰ� UI
    Player1StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Player2StateUI->SetVisibility(ESlateVisibility::Collapsed);
    
    // Enemy �ܰ� UI
    Enemy1StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy2StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy3StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy4StateUI->SetVisibility(ESlateVisibility::Collapsed);

    // Boss �ܰ� UI
    BossHPBar->SetVisibility(ESlateVisibility::Collapsed);
    BossCurrentHPText->SetVisibility(ESlateVisibility::Collapsed);
    BossMaxHPText->SetVisibility(ESlateVisibility::Collapsed);
    BossNameText->SetVisibility(ESlateVisibility::Collapsed);
    RemainHPLine->SetVisibility(ESlateVisibility::Collapsed);
    ShowForwardSlash->SetVisibility(ESlateVisibility::Collapsed);
    BossModeTimer->SetVisibility(ESlateVisibility::Collapsed);
}



void UCooperationWidget::InitPlayerUI(FPlayerData* Player1, FPlayerData* Player2)
{
    Player1StateUI->InitPlayerState(*Player1);
    Player2StateUI->InitPlayerState(*Player2);
}

void UCooperationWidget::UpdatePlayerUI(FPlayerData* Player1, FPlayerData* Player2)
{
    Player1StateUI->UpdateStatus(*Player1);
    Player2StateUI->UpdateStatus(*Player2);
}

void UCooperationWidget::InitEnemyUI(FPlayerData* Enemy1, FPlayerData* Enemy2, FPlayerData* Enemy3, FPlayerData* Enemy4)
{
    Enemy1StateUI->InitPlayerState(*Enemy1);
    Enemy2StateUI->InitPlayerState(*Enemy2);
    Enemy3StateUI->InitPlayerState(*Enemy3);
    Enemy4StateUI->InitPlayerState(*Enemy4);
}

void UCooperationWidget::UpdateEnemyUI(FPlayerData* Enemy1, FPlayerData* Enemy2, FPlayerData* Enemy3, FPlayerData* Enemy4)
{
    Enemy1StateUI->UpdateStatus(*Enemy1);
    Enemy2StateUI->UpdateStatus(*Enemy2);
    Enemy3StateUI->UpdateStatus(*Enemy3);
    Enemy4StateUI->UpdateStatus(*Enemy4);
}

void UCooperationWidget::InitBossUI(const FBossUIData& BossData)
{
    BossHPBar->SetPercent(1.0f);
    BossNameText->SetText(FText::FromString(BossData.BossName));
    BossCurrentHPText->SetText(FText::AsNumber(BossData.MaxHP));
    BossMaxHPText->SetText(FText::AsNumber(BossData.MaxHP));

    int32 RemainHPLineCount = BossRemaingHPMaxLine; 
    FString FormattedText = FString::Printf(TEXT("X%d"), RemainHPLineCount);
    RemainHPLine->SetText(FText::FromString(FormattedText));

}

void UCooperationWidget::UpdateBossUI(const FBossUIData& BossData)
{

    BossCurrentHPText->SetText(FText::AsNumber(BossData.CurrentHP));
    BossMaxHPText->SetText(FText::AsNumber(BossData.MaxHP));

    int32 RemainHPLineCount = BossData.CurrentHP / HPPerLine; 
    float Fraction = static_cast<float>(temp_boss_hp % HPPerLine) / HPPerLine;
    BossHPBar->SetPercent(Fraction);
    if (BossData.CurrentHP > HPPerLine)
    {
        FString FormattedText = FString::Printf(TEXT("X%d"), RemainHPLineCount);
        RemainHPLine->SetText(FText::FromString(FormattedText));
    }
    else
    {
        RemainHPLine->SetText(FText::FromString(TEXT("")));
    }


    if (RemainHPLineCount == 0)
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Black); // �Ǵ� ����
    }
    else
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(ProgressBarColors[RemainHPLineCount % (ProgressBarColors.Num() - 1)]);
    }

}

void UCooperationWidget::UpdateBossTimer(float time)
{
    int32 Minutes = FMath::FloorToInt(time / 60);
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(time, 60));

    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    if (BossModeTimer)
    {
        BossModeTimer->SetText(FText::FromString(TimeString));
    }

}