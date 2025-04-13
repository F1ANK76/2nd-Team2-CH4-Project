// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "../SubWidget/CharacterSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"


//initWidget�� ������� �ʾ� ���� �ӽ� �Լ�...
void UCharacterSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();


    // �ڽ� ������ �� UMapSelectTileWidget�� ���ε�
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
    // ���� ó�� ����: UI ����, ���� ���� ��
    //���Ӹ�忡 �����ϱ�
}