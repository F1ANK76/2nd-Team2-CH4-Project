// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingWidget.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGameSettingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CheckBox1) CheckBox1->SetIsChecked(true);
}

void UGameSettingWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    if (FarmingModeCheckBox) FarmingModeCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnFarmingCheckBoxChanged);
    if (BuffModeCheckBox) BuffModeCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnBuffCheckBoxChanged);
    if (LifePointsModeCheckBox) LifePointsModeCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnLifePointCheckBoxChanged);
    if (TimeLimitModeCheckBox) TimeLimitModeCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnTimeLimitCheckBoxChanged);

    if (LifePointsIncreaseButton) LifePointsIncreaseButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickedIncreaseLifePoints);
    if (LifePointsDecreaseButton) LifePointsDecreaseButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickedDecreaseLifePoints);

    if (CheckBox1) CheckBox1->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnCheckBox1Changed);
    if (CheckBox2) CheckBox2->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnCheckBox2Changed);
    if (CheckBox3) CheckBox3->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnCheckBox3Changed);
    if (CheckBox4) CheckBox4->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnCheckBox4Changed);
    if (CheckBox5) CheckBox5->OnCheckStateChanged.AddDynamic(this, &UGameSettingWidget::OnCheckBox5Changed);
}

void UGameSettingWidget::UncheckAllExcept(UCheckBox* CheckedBox)
{
    if (CheckBox1 && CheckBox1 != CheckedBox) CheckBox1->SetIsChecked(false);
    if (CheckBox2 && CheckBox2 != CheckedBox) CheckBox2->SetIsChecked(false);
    if (CheckBox3 && CheckBox3 != CheckedBox) CheckBox3->SetIsChecked(false);
    if (CheckBox4 && CheckBox4 != CheckedBox) CheckBox4->SetIsChecked(false);
    if (CheckBox5 && CheckBox5 != CheckedBox) CheckBox5->SetIsChecked(false);
}

void UGameSettingWidget::OnCheckBox1Changed(bool bIsChecked)
{
    if (bIsChecked)
    {
        UncheckAllExcept(CheckBox1);
        SelectedTimeIndex = 1;
    }
}

void UGameSettingWidget::OnCheckBox2Changed(bool bIsChecked)
{
    if (bIsChecked)
    {
        UncheckAllExcept(CheckBox2);
        SelectedTimeIndex = 2;
    }
}

void UGameSettingWidget::OnCheckBox3Changed(bool bIsChecked)
{
    if (bIsChecked)
    {
        UncheckAllExcept(CheckBox3);
        SelectedTimeIndex = 3;
    }
}

void UGameSettingWidget::OnCheckBox4Changed(bool bIsChecked)
{
    if (bIsChecked)
    {
        UncheckAllExcept(CheckBox4);
        SelectedTimeIndex = 4;
    }
}

void UGameSettingWidget::OnCheckBox5Changed(bool bIsChecked)
{
    if (bIsChecked)
    {
        UncheckAllExcept(CheckBox5);
        SelectedTimeIndex = 5;
    }
}


void UGameSettingWidget::OnClickedIncreaseLifePoints()
{
    
    if (LifePoint < 9)
    {
        LifePoint++;
        UpdateLifePoints();
    }

}


void UGameSettingWidget::OnClickedDecreaseLifePoints()
{
    if (LifePoint > 1)
    {
        LifePoint--;
        UpdateLifePoints();
    }
    
}


void UGameSettingWidget::UpdateLifePoints()
{
    // LifePoints �ؽ�Ʈ ��Ͽ� �� ����
    if (LifePoints)
    {
        LifePoints->SetText(FText::AsNumber(LifePoint));
    }

    // TODO: ���Ӹ�峪 �ٸ� ���� �ý��� �����ڿ� ����
}


void UGameSettingWidget::OnLifePointCheckBoxChanged(bool bIsChecked)
{
    bIsLifePointModeChecked = bIsChecked;
    UE_LOG(LogTemp, Warning, TEXT("bIsLifePointModeChecked: %s"), bIsLifePointModeChecked ? TEXT("true") : TEXT("false"));

    // TODO: ���Ӹ�峪 �ٸ� ���� �ý��� �����ڿ� ����
}

void UGameSettingWidget::OnFarmingCheckBoxChanged(bool bIsChecked)
{
    bIsFarmingModeChecked = bIsChecked;
    // TODO: ���Ӹ�峪 �ٸ� ���� �ý��� �����ڿ� ����
}

void UGameSettingWidget::OnBuffCheckBoxChanged(bool bIsChecked)
{
    bIsBuffModeChecked = bIsChecked;
    // TODO: ���Ӹ�峪 �ٸ� ���� �ý��� �����ڿ� ����
}

void UGameSettingWidget::OnTimeLimitCheckBoxChanged(bool bIsChecked)
{
    bIsTimeLimitModeChecked = bIsChecked;
    if (bIsChecked)
    {
        CheckBox1->SetIsEnabled(true);  // Ȱ��ȭ
        CheckBox2->SetIsEnabled(true);
        CheckBox3->SetIsEnabled(true);
        CheckBox4->SetIsEnabled(true);
        CheckBox5->SetIsEnabled(true);
        CheckBox1->SetIsChecked(true);
    }
    else
    {
        UncheckAllExcept(nullptr);
        CheckBox1->SetIsEnabled(false); // ��Ȱ��ȭ
        CheckBox2->SetIsEnabled(false);
        CheckBox3->SetIsEnabled(false);
        CheckBox4->SetIsEnabled(false);
        CheckBox5->SetIsEnabled(false);
    }
}
