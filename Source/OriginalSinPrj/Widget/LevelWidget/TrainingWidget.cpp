// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingWidget.h"
#include "Components/Button.h"


//InitWidget�� ���� �۵��� �� ��� ����...
void UTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedPlayerSettingButton);
}

void UTrainingWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
	OpenPlayerSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedPlayerSettingButton);
}

void UTrainingWidget::OnClickedPlayerSettingButton()
{
	
	//setting Ui slide
	if (bIsSlided)
	{
		Super::PlayUIOpenAnimation(SettingWindowSlideInAnim);
		bIsSlided = false;
	}
	else
	{
		Super::PlayUICloseAnimation(SettingWindowSlideOutAnim);
		bIsSlided = true;
	}
	
	//OnClickedOpenWidget(EWidgetType::OptionWidget);
}

