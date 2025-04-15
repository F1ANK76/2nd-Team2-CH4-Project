// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectTileWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMapSelectTileWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	Super::InitWidget(uiSubsystem);

	MapSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
	MapName->SetText(FText::FromString("Name"));

	//FSlateBrush Brush;
	//Brush.SetResourceObject(SomeTexture);
	//MapImage->SetBrush(Brush);
}

void UMapSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnMapSelectTileClicked.Broadcast(MyTileIndex); // ��ư ������ �̺�Ʈ ����
}