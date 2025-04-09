// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"


//InitWidget이 아직 작동할 수 없어서 만든...
void UTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}

void UTrainingWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//각 버튼에 클릭시 발동?될 함수 연결하기
	/*
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	*/
	//각 버튼에 클릭시 발동?될 함수 연결하기
	ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}

void UTrainingWidget::OnClickedPlayerSettingButton()
{
	/*
	//setting Ui slide
	if (bIsSlided)
	{
		Super::PlayUIOpenAnimation(SettingWindowSlideInAnim);
	}
	else
	{
		Super::PlayUICloseAnimation(SettingWindowSlideOutAnim);
	}
	*/
	//OnClickedOpenWidget(EWidgetType::OptionWidget);
}

