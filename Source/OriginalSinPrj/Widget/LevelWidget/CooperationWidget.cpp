// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperationWidget.h"
#include "../AddedWidget/PlayerStateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"


void UCooperationWidget::NativeConstruct()
{
    ////////////////////////////임시버튼/////////////////////////////////
    if (tempDamage)
        tempDamage->OnClicked.AddDynamic(this, &UCooperationWidget::damage);
    if (temp_boss_init)
        temp_boss_init->OnClicked.AddDynamic(this, &UCooperationWidget::initHP);
    if (temp_boss_die)
        temp_boss_die->OnClicked.AddDynamic(this, &UCooperationWidget::dieHP);

    if (StageButton1)
        StageButton1->OnClicked.AddDynamic(this, &UCooperationWidget::ActiveStage1Widget);
    if (StageButton2)
        StageButton2->OnClicked.AddDynamic(this, &UCooperationWidget::ActiveStage2Widget);
    if (StageButton3)
        StageButton3->OnClicked.AddDynamic(this, &UCooperationWidget::ActiveStage3Widget);

    if (StartTimerButton)
        StartTimerButton->OnClicked.AddDynamic(this, &UCooperationWidget::StartStageTimer);

    if (StopTimerButton)
        StopTimerButton->OnClicked.AddDynamic(this, &UCooperationWidget::StopStageTimer);
    ////////////////////////////임시버튼/////////////////////////////////



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

    DeactivateAllWidgets();
}



void UCooperationWidget::ActiveWidgetbyStage(int32 StageIndex)
{
    switch (StageIndex)
    {
    case 1:
        // 1단계 위젯 활성화
        ActiveStage1Widget();
        break;

    case 2:
        // 2단계 위젯 활성화
        ActiveStage1Widget();
        ActiveStage2Widget();
        break;

    case 3:
        // 3단계 위젯 활성화
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
    // 1단계 위젯 활성화
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
    // Player 단계 UI
    Player1StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Player2StateUI->SetVisibility(ESlateVisibility::Collapsed);

    // Enemy 단계 UI
    Enemy1StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy2StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy3StateUI->SetVisibility(ESlateVisibility::Collapsed);
    Enemy4StateUI->SetVisibility(ESlateVisibility::Collapsed);

    // Boss 단계 UI
    BossHPBar->SetVisibility(ESlateVisibility::Collapsed);
    BossCurrentHPText->SetVisibility(ESlateVisibility::Collapsed);
    BossMaxHPText->SetVisibility(ESlateVisibility::Collapsed);
    BossNameText->SetVisibility(ESlateVisibility::Collapsed);
    RemainHPLine->SetVisibility(ESlateVisibility::Collapsed);
    ShowForwardSlash->SetVisibility(ESlateVisibility::Collapsed);
    BossModeTimer->SetVisibility(ESlateVisibility::Collapsed);
}




//GameMode 혹은 Subsystem쪽에서 초기화 해주면, 자동으로 UI설정값이 들어가게 되는 함수.

void UCooperationWidget::InitPlayerUI(FPlayerData* Player1, FPlayerData* Player2)
{
    Player1StateUI->InitPlayerState(*Player1);
    Player2StateUI->InitPlayerState(*Player2);
}

//GameMode 혹은 Subsystem쪽에서 업데이트 해주면, UI의 값을 바꾸는 함수.
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

void UCooperationWidget::InitBossUI(FBossUIData* BossData)
{
    BossHPBar->SetPercent(1.0f);
    BossNameText->SetText(FText::FromString(BossData->BossName));
    BossCurrentHPText->SetText(FText::AsNumber(BossData->MaxHP));
    BossMaxHPText->SetText(FText::AsNumber(BossData->MaxHP));

    int32 RemainHPLineCount = BossRemaingHPMaxLine; // 계산식;
    FString FormattedText = FString::Printf(TEXT("X%d"), RemainHPLineCount);
    RemainHPLine->SetText(FText::FromString(FormattedText));

}

void UCooperationWidget::UpdateBossUI(FBossUIData* BossData)
{

    BossCurrentHPText->SetText(FText::AsNumber(BossData->CurrentHP));
    BossMaxHPText->SetText(FText::AsNumber(BossData->MaxHP));

    int32 RemainHPLineCount = BossData->CurrentHP / HPPerLine; // 계산식;    .
    float Fraction = static_cast<float>(temp_boss_hp % HPPerLine) / HPPerLine;
    BossHPBar->SetPercent(Fraction);
    if (BossData->CurrentHP > HPPerLine)
    {
        FString FormattedText = FString::Printf(TEXT("X%d"), RemainHPLineCount);
        RemainHPLine->SetText(FText::FromString(FormattedText));
    }
    else
    {
        RemainHPLine->SetText(FText::FromString(TEXT("")));
    }


    // 마지막 체력줄일 경우, 특수 색상 적용
    if (RemainHPLineCount == 0)
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Black); // 또는 숨김
    }
    else
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(ProgressBarColors[RemainHPLineCount % (ProgressBarColors.Num() - 1)]);
    }

}



void UCooperationWidget::StartStageTimer()
{
    ElapsedTime = 0.0f;

    // 1초 간격으로 TickStageTimer 호출
    GetWorld()->GetTimerManager().SetTimer(
        StageTimerHandle,
        this,
        &UCooperationWidget::TickStageTimer,
        1.0f,
        true
    );
}


void UCooperationWidget::TickStageTimer()
{
    ElapsedTime += 1.0f;

    int32 Minutes = FMath::FloorToInt(ElapsedTime / 60);
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(ElapsedTime, 60));

    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    if (BossModeTimer)
    {
        BossModeTimer->SetText(FText::FromString(TimeString));
    }
}


void UCooperationWidget::StopStageTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(StageTimerHandle);
}



/////////////////////////////////test///////////////////////////////////
//임시함수 테스트용
void UCooperationWidget::damage()
{
    temp_boss_hp = temp_boss_hp - 3;
    if (temp_boss_hp < 0)
    {
        temp_boss_hp = 0;
    }
    tempUpdateBossUI();
}



void UCooperationWidget::initHP()
{
    BossCurrentHPText->SetText(FText::AsNumber(temp_boss_hp));
    BossMaxHPText->SetText(FText::AsNumber(max_boss_hp));
    FString FormattedText = FString::Printf(TEXT("X%d"), BossRemaingHPMaxLine);
    RemainHPLine->SetText(FText::FromString(FormattedText));

    BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(ProgressBarColors[BossRemaingHPMaxLine % (ProgressBarColors.Num() - 1)]);
    BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(BossRemaingHPMaxLine + 1) % (ProgressBarColors.Num() - 1)]);

}

void UCooperationWidget::dieHP()
{
    temp_boss_hp = 20;
    tempUpdateBossUI();
}


void UCooperationWidget::tempUpdateBossUI()
{
    BossCurrentHPText->SetText(FText::AsNumber(temp_boss_hp));
    BossMaxHPText->SetText(FText::AsNumber(max_boss_hp));


    int32 RemainHPLineCount = temp_boss_hp / HPPerLine; // 계산식;
    float Fraction = static_cast<float>(temp_boss_hp % HPPerLine) / HPPerLine;
    BossHPBar->SetPercent(Fraction);

    if (temp_boss_hp > HPPerLine)
    {
        FString FormattedText = FString::Printf(TEXT("X%d"), RemainHPLineCount);
        RemainHPLine->SetText(FText::FromString(FormattedText));

    }
    else
    {
        RemainHPLine->SetText(FText::FromString(TEXT("")));
    }

    // 마지막 체력줄일 경우, 특수 색상 적용
    if (RemainHPLineCount == 0)
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(ProgressBarColors.Last()); // 또는 숨김
    }
    else
    {
        BossHPBar->WidgetStyle.FillImage.TintColor = FSlateColor(ProgressBarColors[(RemainHPLineCount + 1) % (ProgressBarColors.Num() - 1)]);
        BossHPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(ProgressBarColors[RemainHPLineCount % (ProgressBarColors.Num() - 1)]);
    }
}