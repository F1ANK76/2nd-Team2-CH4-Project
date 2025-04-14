// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectTileWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

//InitWidget이 아직 작동할 수 없어서 만든...
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
    OnMapSelectTileClicked.Broadcast(MyTileIndex); // 버튼 눌리면 이벤트 날림
}