// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "../SubWidget/CharacterSelectTileWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "../GameInstance/UISubsystem.h"
#include "OriginalSinPrj/GameInstance/DataSubsystem.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterTypeData.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterDataStruct.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"



//initWidget�� ������� �ʾ� ���� �ӽ� �Լ�...
void UCharacterSelectWidget::InitWidget(UUISubsystem* uiHandle)
{
    Super::InitWidget(uiHandle);

    InitDelegate();
    InitCharacterTiles();
    UICloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);

    CheckValidOfGameInstnace();
}

void UCharacterSelectWidget::OnTileClickedFromTile(ECharacterType SelectedType)
{
    PlayUIEffectSound(EUISfxSoundType::Click);
    checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

    ELevelType CurrentLevel = UIHandle->GetCurrentLevelType();

    if (CurrentLevel == ELevelType::TitleLevel)
    {
        UIHandle->OnClickedMoveLevel(ELevelType::FarmingLevel, true);
        /*OnClickedCloseWidget(EAddWidgetType::CharacterSelectWidget);
        OnClickedOpenWidget(EAddWidgetType::MapSelectWidget);*/
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
        //OnClickedCloseWidget(EAddWidgetType::CharacterSelectWidget);
    }

    if (CheckValidOfGameInstnace())
    {
        GameInstance->SetSelectedCharacterType(SelectedType);
    }
    //TODO : Character Type Send To GameInstance
    
    // ���� ó�� ����: UI ����, ���� ���� ��
    //���Ӹ�忡 �����ϱ�
}

void UCharacterSelectWidget::OnClickedClose()
{
    PlayUIEffectSound(EUISfxSoundType::Close);
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
            Tiles.Add(Tile);
            Tile->InitWidget(UIHandle);
            Tile->OnCharacterSelectTileClicked.AddDynamic(this, &UCharacterSelectWidget::OnTileClickedFromTile);

        }
    }
}

void UCharacterSelectWidget::InitCharacterTiles()
{
    if (!IsValid(GetGameInstance()))
    {
        return;
    }

    UDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<UDataSubsystem>();

    if (!IsValid(DataSubsystem))
    {
        return;
    }

    for (int32 i = 0; i < Tiles.Num(); i++)
    {
        ECharacterType TargetType = (ECharacterType)(i % DataSubsystem->GetCharacterTypeSize());

        const FCharacterDataStruct* TargetData = DataSubsystem->GetCharacterDataByType(TargetType);

        Tiles[i]->SetCharacterType(TargetType);
        Tiles[i]->SetCharacterImage(TargetData->PortraitImage.LoadSynchronous());
        Tiles[i]->SetCharacterName(TargetData->CharacterName);
    }
}

bool UCharacterSelectWidget::CheckValidOfGameInstnace()
{
    if (IsValid(GameInstance))
    {
        return true;
    }

    if (!IsValid(GetGameInstance()))
    {
        return false;
    }

    GameInstance = GetGameInstance<UOriginalSinPrjGameInstance>();

    if (!IsValid(GameInstance))
    {
        return false;
    }

    return true;
}
