// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectTileWidget.h"
#include "Components/Button.h"

//InitWidget�� ���� �۵��� �� ��� ����...
void UCharacterSelectTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}

void UCharacterSelectTileWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	/*
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	*/
	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}



void UCharacterSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnCharacterSelectTileClicked.Broadcast(MyTileIndex); // ��ư ������ �̺�Ʈ ����
}

