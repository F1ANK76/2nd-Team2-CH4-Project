// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectTileWidget.h"
#include "Components/Button.h"

//InitWidget이 아직 작동할 수 없어서 만든...
void UCharacterSelectTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}

void UCharacterSelectTileWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//각 버튼에 클릭시 발동?될 함수 연결하기
	/*
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	*/
	//각 버튼에 클릭시 발동?될 함수 연결하기
	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}



void UCharacterSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnCharacterSelectTileClicked.Broadcast(MyTileIndex); // 버튼 눌리면 이벤트 날림
}

