// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"


void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
	/*
	//각 버튼에 클릭시 발동?될 함수 연결하기
	SinglePlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	TrainingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTrainingMode);
	MultiPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMultiMode);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
	*/
}

void UTitleWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//각 버튼에 클릭시 발동?될 함수 연결하기
	SinglePlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	TrainingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTrainingMode);
	MultiPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMultiMode);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}

void UTitleWidget::OnClickedOptionButton()
{
	//OnClickedOpenWidget(EWidgetType::OptionWidget);
}