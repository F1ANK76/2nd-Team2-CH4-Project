// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "../SubWidget/MapSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"


void UMapSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 자식 위젯들 중 UMapSelectTileWidget만 바인딩
    TArray<UWidget*> FoundWidgets;
    WidgetTree->GetAllWidgets(FoundWidgets);

    for (UWidget* Widget : FoundWidgets)
    {
        UMapSelectTileWidget* Tile = Cast<UMapSelectTileWidget>(Widget);
        if (Tile)
        {
            Tile->OnMapSelectTileClicked.AddDynamic(this, &UMapSelectWidget::OnTileClickedFromTile);
        }
    }
}

void UMapSelectWidget::InitWidget(UUIHandle* NewUIHandle)
{
    Super::InitWidget(NewUIHandle);


    // 자식 위젯들 중 UMapSelectTileWidget만 바인딩
    TArray<UWidget*> FoundWidgets;
    WidgetTree->GetAllWidgets(FoundWidgets);

    for (UWidget* Widget : FoundWidgets)
    {
        UMapSelectTileWidget* Tile = Cast<UMapSelectTileWidget>(Widget);
        if (Tile)
        {
            Tile->OnMapSelectTileClicked.AddDynamic(this, &UMapSelectWidget::OnTileClickedFromTile);
        }
    }
}


void UMapSelectWidget::OnTileClickedFromTile(int32 TileIndex)
{
    UE_LOG(LogTemp, Log, TEXT("Tile with index %d was clicked!"), TileIndex);
    // 선택 처리 로직: UI 강조, 변수 저장 등
}