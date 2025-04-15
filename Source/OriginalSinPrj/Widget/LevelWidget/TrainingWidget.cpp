// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingWidget.h"
#include "Components/Button.h"


void UTrainingWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	Super::InitWidget(uiSubsystem);

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

