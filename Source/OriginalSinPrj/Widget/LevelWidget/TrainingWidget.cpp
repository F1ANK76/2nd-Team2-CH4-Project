// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingWidget.h"
#include "Components/Button.h"


//InitWidget이 아직 작동할 수 없어서 만든...
void UTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToTitleButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedMoveTitle);
	OpenPlayerSettingButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedPlayerSettingButton);
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

