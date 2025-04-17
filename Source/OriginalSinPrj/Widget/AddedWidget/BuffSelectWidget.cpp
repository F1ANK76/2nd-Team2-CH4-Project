// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "GameInstance/LevelSubsystem.h"
#include "OriginalSinPrj/GameInstance/DataSubsystem.h"
#include "GameState/CooperationGameState.h"
#include "GameMode/CooperationGameMode.h"
#include "OriginalSinPrj/Public/BuffSelectManager.h"
#include "OriginalSinPrj/GameInstance/Struct/BuffDataStruct.h"
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
    UDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
    if (DataSubsystem)
    {
        Data = DataSubsystem->GetBuffDataArray();
        // 사용...
    }
}

void UBuffSelectWidget::InitializeBuffs(TArray<EBuffType> BuffList)
{
    ReceivedBuffType.Add(BuffList[0]);
    ReceivedBuffType.Add(BuffList[1]);
    ReceivedBuffType.Add(BuffList[2]);

    BuffButton1->SetVisibility(ESlateVisibility::Visible);
    BuffButton2->SetVisibility(ESlateVisibility::Visible);
    BuffButton3->SetVisibility(ESlateVisibility::Visible);

    
    for (const FBuffDataStruct* BuffData : Data)
    {
        
        if (!BuffData) continue;
        if (BuffData->BuffType == BuffList[0])
        {
            if (BuffName1)
            {
                FText DisplayText = StaticEnum<EBuffType>()->GetDisplayNameTextByValue((int64)BuffData->BuffType);
                BuffName1->SetText(DisplayText);
            }
            if (Buff1InfoText)
            {
                Buff1InfoText->SetText(FText::FromString(BuffData->Description));
            }
            if (BuffImage1 && BuffData->BuffTexture) // BuffIcon이 UTexture2D*
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(BuffData->BuffTexture);
                Brush.ImageSize = FVector2D(64, 64); // 필요 시 사이즈 조정
                BuffImage1->SetBrush(Brush);
            }
        }
        else if (BuffData->BuffType == BuffList[1])
        {
            if (BuffName2)
            {
                FText DisplayText = StaticEnum<EBuffType>()->GetDisplayNameTextByValue((int64)BuffData->BuffType);
                BuffName2->SetText(DisplayText);
            }
            if (Buff2InfoText)
            {
                Buff2InfoText->SetText(FText::FromString(BuffData->Description));
            }
            if (BuffImage2 && BuffData->BuffTexture) // BuffIcon이 UTexture2D*
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(BuffData->BuffTexture);
                Brush.ImageSize = FVector2D(64, 64); // 필요 시 사이즈 조정
                BuffImage2->SetBrush(Brush);
            }
        }
        else if (BuffData->BuffType == BuffList[2])
        {
            if (BuffName3)
            {
                FText DisplayText = StaticEnum<EBuffType>()->GetDisplayNameTextByValue((int64)BuffData->BuffType);
                BuffName3->SetText(DisplayText);
            }
            if (Buff3InfoText)
            {
                Buff3InfoText->SetText(FText::FromString(BuffData->Description));
            }
            if (BuffImage3 && BuffData->BuffTexture) // BuffIcon이 UTexture2D*
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(BuffData->BuffTexture);
                Brush.ImageSize = FVector2D(64, 64); // 필요 시 사이즈 조정
                BuffImage3->SetBrush(Brush);
            }
        }
    }





}

void UBuffSelectWidget::OnBuff1Clicked()
{

    DeactivateOhterWidgets(1, ReceivedBuffType[0]);
    //play animation
    //UE_LOG(LogTemp, Log, TEXT("Buff 1 Selected: %s"), *Buffs[0].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff2Clicked()
{
    DeactivateOhterWidgets(2, ReceivedBuffType[1]);
    //play animations
    //UE_LOG(LogTemp, Log, TEXT("Buff 2 ���õ�: %s"), *Buffs[1].BuffName.ToString());
}

void UBuffSelectWidget::OnBuff3Clicked()
{
    DeactivateOhterWidgets(3, ReceivedBuffType[2]);
    //play animation
    //UE_LOG(LogTemp, Log, TEXT("Buff 3 ���õ�: %s"), *Buffs[2].BuffName.ToString());

}

void UBuffSelectWidget::DeactivateOhterWidgets(int32 SelectedButtonIndex, EBuffType SelectedBuff)
{

    BuffButton1->SetVisibility(ESlateVisibility::Collapsed);
    BuffButton2->SetVisibility(ESlateVisibility::Collapsed);
    BuffButton3->SetVisibility(ESlateVisibility::Collapsed);

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AWitchController* MyPC = Cast<AWitchController>(PC))
        {
            MyPC->Server_SelectBuff(SelectedButtonIndex, SelectedBuff);
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
