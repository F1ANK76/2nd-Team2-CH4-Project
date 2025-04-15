// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "../SubWidget/MapSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "../GameInstance/UISubsystem.h"


void UMapSelectWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);

    TArray<UWidget*> FoundWidgets;
    WidgetTree->GetAllWidgets(FoundWidgets);

    for (UWidget* Widget : FoundWidgets)
    {
        if (!IsValid(Widget))
        {
            continue;
        }

        UMapSelectTileWidget* Tile = Cast<UMapSelectTileWidget>(Widget);

        if (IsValid(Tile))
        {
            Tile->OnMapSelectTileClicked.AddDynamic(this, &UMapSelectWidget::OnTileClickedFromTile);
            Tile->InitWidget(UIHandle);
        }
    }

    UICloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedBack);
}

void UMapSelectWidget::OnTileClickedFromTile(int32 TileIndex)
{
    UIHandle->OnClickedMoveLevel(ELevelType::FarmingLevel, true);
    // Requset Move Level To GameMode or GameInstance
    UE_LOG(LogTemp, Log, TEXT("Tile with index %d was clicked!"), TileIndex);
    // ���� ó�� ����: UI ����, ���� ���� ��

    //���Ӹ�峪 �̷����ٰ� �����ϱ�
}

void UMapSelectWidget::OnClickedBack()
{
    //OnClickedCloseWidget(EAddWidgetType::MapSelectWidget);
    checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
    ELevelType CurrentLevel = UIHandle->GetCurrentLevelType();

    if (CurrentLevel == ELevelType::TitleLevel)
    {
        OnClickedCloseWidget(EAddWidgetType::MapSelectWidget);
        OnClickedOpenWidget(EAddWidgetType::CharacterSelectWidget);
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);

    }
}
