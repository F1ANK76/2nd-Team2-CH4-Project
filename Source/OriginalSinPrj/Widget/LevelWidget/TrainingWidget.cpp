// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"


//InitWidget�� ���� �۵��� �� ��� ����...
void UTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}

void UTrainingWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	/*
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);
	*/
	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
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

