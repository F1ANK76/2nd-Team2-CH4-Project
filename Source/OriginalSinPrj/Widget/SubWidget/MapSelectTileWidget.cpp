// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectTileWidget.h"
#include "Components/Button.h"

//InitWidget�� ���� �۵��� �� ��� ����...
void UMapSelectTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MapSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}

void UMapSelectTileWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	/*
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	*/
	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	MapSelectButton->OnClicked.AddDynamic(this, &ThisClass::HandleButtonClicked);
}

void UMapSelectTileWidget::HandleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Tile with was clicked!"));
    OnMapSelectTileClicked.Broadcast(MyTileIndex); // ��ư ������ �̺�Ʈ ����
}