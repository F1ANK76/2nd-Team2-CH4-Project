// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectTileWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


//InitWidget�� ���� �۵��� �� ��� ����...
void UCharacterSelectTileWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	UE_LOG(LogTemp, Warning, TEXT("AddDynamic Button"));
	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);

	//FSlateBrush Brush;
	//Brush.SetResourceObject(SomeTexture);
	//CharacterImage->SetBrush(Brush);

	CharacterName->SetText(FText::FromString("Name"));
}

void UCharacterSelectTileWidget::SetCharacterType(ECharacterType Type)
{
	CharacterType = Type;
}

void UCharacterSelectTileWidget::SetCharacterImage(UTexture2D* CharacterImg)
{
	CharacterImage->SetBrushFromTexture(CharacterImg);
}

void UCharacterSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnCharacterSelectTileClicked.Broadcast(MyTileIndex); // ��ư ������ �̺�Ʈ ����
}


