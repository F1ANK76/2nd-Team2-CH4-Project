// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "../SubWidget/CharacterSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "../GameInstance/UISubsystem.h"


//initWidget�� ������� �ʾ� ���� �ӽ� �Լ�...
void UCharacterSelectWidget::InitWidget(UUISubsystem* uiHandle)
{
    Super::InitWidget(uiHandle);

    InitDelegate();
    UICloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);
}

void UCharacterSelectWidget::OnTileClickedFromTile(int32 TileIndex)
{
    checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

    ELevelType CurrentLevel = UIHandle->GetCurrentLevelType();

    if (CurrentLevel == ELevelType::TitleLevel)
    {
        OnClickedCloseWidget(EAddWidgetType::CharacterSelectWidget);
        OnClickedOpenWidget(EAddWidgetType::MapSelectWidget);
    }
    

    //TODO : Character Index Send To GameMode
    UE_LOG(LogTemp, Log, TEXT("Tile with index %d was clicked!"), TileIndex);
    // ���� ó�� ����: UI ����, ���� ���� ��
    //���Ӹ�忡 �����ϱ�
}

void UCharacterSelectWidget::OnClickedClose()
{
    checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

    //OnClickedCloseWidget(EAddWidgetType::CharacterSelectWidget);

    ELevelType CurrentLevel = UIHandle->GetCurrentLevelType();

    if (CurrentLevel == ELevelType::TitleLevel)
    {
        OnClickedCloseWidget(EAddWidgetType::CharacterSelectWidget);
        UIHandle->SetVisibilityWidget(true);
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UCharacterSelectWidget::InitDelegate()
{
    TArray<UWidget*> FoundWidgets;
    WidgetTree->GetAllWidgets(FoundWidgets);

    for (UWidget* Widget : FoundWidgets)
    {
        if (!IsValid(Widget))
        {
            continue;
        }

        UCharacterSelectTileWidget* Tile = Cast<UCharacterSelectTileWidget>(Widget);

        if (IsValid(Tile))
        {
            Tile->InitWidget(UIHandle);
            Tile->OnCharacterSelectTileClicked.AddDynamic(this, &UCharacterSelectWidget::OnTileClickedFromTile);
        }
    }
}
