// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectTileWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

//InitWidget이 아직 작동할 수 없어서 만든...
void UCharacterSelectTileWidget::NativeConstruct()
{
	Super::NativeConstruct();


	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);

	//FSlateBrush Brush;
	//Brush.SetResourceObject(SomeTexture);
	//CharacterImage->SetBrush(Brush);

	CharacterName->SetText(FText::FromString("Name"));
}




void UCharacterSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnCharacterSelectTileClicked.Broadcast(MyTileIndex); // 버튼 눌리면 이벤트 날림
}

