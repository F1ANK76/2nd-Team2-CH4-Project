// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"


void UBuffSelectWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);

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
    DeactivateOhterWidgets(1);
    //play animation
    UE_LOG(LogTemp, Log, TEXT("Buff 1 ���õ�: %s"), *Buffs[0].BuffName.ToString());
    //���Ӹ�� ����
}

void UBuffSelectWidget::OnBuff2Clicked()
{
    DeactivateOhterWidgets(2);
    //play animation
    UE_LOG(LogTemp, Log, TEXT("Buff 2 ���õ�: %s"), *Buffs[1].BuffName.ToString());
    //���Ӹ�� ����
}

void UBuffSelectWidget::OnBuff3Clicked()
{
    DeactivateOhterWidgets(3);
    //play animation
    UE_LOG(LogTemp, Log, TEXT("Buff 3 ���õ�: %s"), *Buffs[2].BuffName.ToString());
    //���Ӹ�� ����
}

void UBuffSelectWidget::DeactivateOhterWidgets(int32 SelectedButtonIndex)
{
    //��ư ���� ������ �ΰ� �������� �� ������.
    switch (SelectedButtonIndex)
    {
    case 1:
        BuffButton2->SetIsEnabled(false); // ��ư ��Ȱ��ȭ (Ŭ�� �� ��)
        BuffButton3->SetIsEnabled(false);
        break;

    case 2:
        BuffButton1->SetIsEnabled(false);
        BuffButton3->SetIsEnabled(false);
        break;

    case 3:
        BuffButton1->SetIsEnabled(false);
        BuffButton2->SetIsEnabled(false);
        break;

    default:
        break;
    }
    BuffButton2->SetIsEnabled(false); // ��ư ��Ȱ��ȭ (Ŭ�� �� ��)
    BuffButton3->SetIsEnabled(false);  // ��ư Ȱ��ȭ (Ŭ�� ����)

}

