// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "GameInstance/LevelSubsystem.h"
#include "GameState/CooperationGameState.h"
#include "GameMode/CooperationGameMode.h"
#include "OriginalSinPrj/Public/BuffSelectManager.h"
#include "OriginalSinPrj/Public/Player/Controller/WitchController.h"
#include "EngineUtils.h"   

void UBuffSelectWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);

    BuffButton1->OnClicked.AddDynamic(this, &ThisClass::OnBuff1Clicked);
    BuffButton1->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButton1);
    BuffButton1->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHoveredButton1);

    BuffButton2->OnClicked.AddDynamic(this, &ThisClass::OnBuff2Clicked);
    BuffButton2->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButton2);
    BuffButton2->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHoveredButton2);
    
    BuffButton3->OnClicked.AddDynamic(this, &ThisClass::OnBuff3Clicked);
    BuffButton3->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButton3);
    BuffButton3->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHoveredButton3);



    BuffButton1->SetVisibility(ESlateVisibility::Visible);
    BuffButton2->SetVisibility(ESlateVisibility::Visible);
    BuffButton3->SetVisibility(ESlateVisibility::Visible);

    Buff1InfoBox->SetVisibility(ESlateVisibility::Collapsed);
    Buff2InfoBox->SetVisibility(ESlateVisibility::Collapsed);
    Buff3InfoBox->SetVisibility(ESlateVisibility::Collapsed);

    InitializeBuffs();
}

void UBuffSelectWidget::InitializeBuffs()
{
    BuffTable.Add(FBuffInfo{ FText::FromString(TEXT("Initial Buff1 Name")),FText::FromString(TEXT("Initial Buff1 Script")),NULL });
    BuffTable.Add(FBuffInfo{ FText::FromString(TEXT("Initial Buff2 Name")),FText::FromString(TEXT("Initial Buff2 Script")),NULL });
    BuffTable.Add(FBuffInfo{ FText::FromString(TEXT("Initial Buff3 Name")),FText::FromString(TEXT("Initial Buff3 Script")),NULL });


//버프 종류는 여기서 결정.
//버프 종류가 기록되어있는 목록을 받아서, 겹치지 않게 세개를 받아 선택지에 올리기
/*
TArray<FBuffType> Buff = {}; //  { 1번, 2번, ... , n번 };
TArray<FBuffType> SelectedBuff;
if (BuffTable.Num() > 0)
{
    int RandomIndex = FMath::RandRange(0, BuffTable.Num() - 1);
}

for (int i = 0; i < 3; i++)
{
    int32 Index = FMath::RandRange(0, Buff.Num() - 1);
    SelectedBuff.Add(Buff[Index]);
    Buff.RemoveAt(Index);
}
*/
//PlayerController?->UISubsystem->OpenWidget(EWidgetType::BuffSelectWidget);
//PlayerController?->BuffSelectWidget->InitializeBuffs(SelectedBuff);


}

void UBuffSelectWidget::OnBuff1Clicked()
{
    DeactivateOhterWidgets(1, BuffTable[0]);
    //play animation
    //UE_LOG(LogTemp, Log, TEXT("Buff 1 Selected: %s"), *Buffs[0].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff2Clicked()
{
    DeactivateOhterWidgets(2, BuffTable[1]);
    //play animation
    //UE_LOG(LogTemp, Log, TEXT("Buff 2 ���õ�: %s"), *Buffs[1].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff3Clicked()
{
    DeactivateOhterWidgets(3, BuffTable[2]);
    //play animation
    //UE_LOG(LogTemp, Log, TEXT("Buff 3 ���õ�: %s"), *Buffs[2].BuffName.ToString());

}

void UBuffSelectWidget::DeactivateOhterWidgets(int32 SelectedButtonIndex, FBuffInfo buff)
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AWitchController* MyPC = Cast<AWitchController>(PC))
        {
            MyPC->Server_SelectBuff(SelectedButtonIndex, buff);
        }
    }
}

void UBuffSelectWidget::OnHoveredButton1()
{
    UE_LOG(LogTemp, Warning, TEXT("Button1Hover"));

    Buff1InfoBox->SetVisibility(ESlateVisibility::Visible);
}

void UBuffSelectWidget::OnUnHoveredButton1()
{
    UE_LOG(LogTemp, Warning, TEXT("Button1UnHover"));

    Buff1InfoBox->SetVisibility(ESlateVisibility::Collapsed);
}


void UBuffSelectWidget::OnHoveredButton2()
{
    UE_LOG(LogTemp, Warning, TEXT("Button2Hover"));

    Buff2InfoBox->SetVisibility(ESlateVisibility::Visible);
}

void UBuffSelectWidget::OnUnHoveredButton2()
{
    UE_LOG(LogTemp, Warning, TEXT("Button2UnHover"));

    Buff2InfoBox->SetVisibility(ESlateVisibility::Collapsed);
}



void UBuffSelectWidget::OnHoveredButton3()
{
    UE_LOG(LogTemp, Warning, TEXT("Button3Hover"));

    Buff3InfoBox->SetVisibility(ESlateVisibility::Visible);
}

void UBuffSelectWidget::OnUnHoveredButton3()
{
    UE_LOG(LogTemp, Warning, TEXT("Button3UnHover"));

    Buff3InfoBox->SetVisibility(ESlateVisibility::Collapsed);
}
