// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "../SubWidget/CharacterSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"


//initWidget이 실행되지 않아 만든 임시 함수...
void UCharacterSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();


    // 자식 위젯들 중 UMapSelectTileWidget만 바인딩
    TArray<UWidget*> FoundWidgets;
    WidgetTree->GetAllWidgets(FoundWidgets);

    for (UWidget* Widget : FoundWidgets)
    {
        UCharacterSelectTileWidget* Tile = Cast<UCharacterSelectTileWidget>(Widget);
        if (Tile)
        {
            Tile->OnCharacterSelectTileClicked.AddDynamic(this, &UCharacterSelectWidget::OnTileClickedFromTile);
        }
    }
}




void UCharacterSelectWidget::OnTileClickedFromTile(int32 TileIndex)
{
    UE_LOG(LogTemp, Log, TEXT("Tile with index %d was clicked!"), TileIndex);
    // 선택 처리 로직: UI 강조, 변수 저장 등
    //게임모드에 전달하기
}