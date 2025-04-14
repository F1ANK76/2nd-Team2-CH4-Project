// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectTileWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

//InitWidget�� ���� �۵��� �� ��� ����...
void UMapSelectTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MapSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);

	//FSlateBrush Brush;
	//Brush.SetResourceObject(SomeTexture);
	//MapImage->SetBrush(Brush);


	MapName->SetText(FText::FromString("Name"));
}

void UMapSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnMapSelectTileClicked.Broadcast(MyTileIndex); // ��ư ������ �̺�Ʈ ����
}