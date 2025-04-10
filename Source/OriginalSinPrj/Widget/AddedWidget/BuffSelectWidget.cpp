// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UBuffSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BuffButton1->OnClicked.RemoveDynamic(this, &UBuffSelectWidget::OnBuff1Clicked);
    BuffButton2->OnClicked.RemoveDynamic(this, &UBuffSelectWidget::OnBuff2Clicked);
    BuffButton3->OnClicked.RemoveDynamic(this, &UBuffSelectWidget::OnBuff3Clicked);

    BuffButton1->OnClicked.AddDynamic(this, &UBuffSelectWidget::OnBuff1Clicked);
    BuffButton2->OnClicked.AddDynamic(this, &UBuffSelectWidget::OnBuff2Clicked);
    BuffButton3->OnClicked.AddDynamic(this, &UBuffSelectWidget::OnBuff3Clicked);
}

void UBuffSelectWidget::InitializeBuffs(const TArray<FBuffInfo>& InBuffs)
{
    Buffs = InBuffs;

    if (Buffs.Num() >= 1)
    {
        BuffName1->SetText(Buffs[0].BuffName);
        BuffDesc1->SetText(Buffs[0].BuffDescription);
        BuffImage1->SetBrushFromTexture(Buffs[0].BuffIcon);
    }

    if (Buffs.Num() >= 2)
    {
        BuffName2->SetText(Buffs[1].BuffName);
        BuffDesc2->SetText(Buffs[1].BuffDescription);
        BuffImage2->SetBrushFromTexture(Buffs[1].BuffIcon);
    }

    if (Buffs.Num() >= 3)
    {
        BuffName3->SetText(Buffs[2].BuffName);
        BuffDesc3->SetText(Buffs[2].BuffDescription);
        BuffImage3->SetBrushFromTexture(Buffs[2].BuffIcon);
    }
}

void UBuffSelectWidget::OnBuff1Clicked()
{
    UE_LOG(LogTemp, Log, TEXT("Buff 1 º±≈√µ : %s"), *Buffs[0].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff2Clicked()
{
    UE_LOG(LogTemp, Log, TEXT("Buff 2 º±≈√µ : %s"), *Buffs[1].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff3Clicked()
{
    UE_LOG(LogTemp, Log, TEXT("Buff 3 º±≈√µ : %s"), *Buffs[2].BuffName.ToString());
}