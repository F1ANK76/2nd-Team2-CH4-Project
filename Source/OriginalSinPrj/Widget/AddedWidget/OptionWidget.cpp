// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"

#include "../GameInstance/AudioSubsystem.h"

void UOptionWidget::NativeConstruct()
{
    Super::NativeConstruct();
	/*
	OpenAnimation = OpenOptionAnim;
	CloseAnimation = CloseOptionAnim;
	*/
	/*
	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
	OpenKeySettingButton->OnClicked.AddDynamic(this, &ThisClass::OpenKeySetting);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
	*/
	CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedCloseOption);
	BGMVolumeSlider->OnValueChanged.RemoveDynamic(this, &ThisClass::OnClickedBGMVolume);
	EffectVolumeSlider->OnValueChanged.RemoveDynamic(this, &ThisClass::OnClickedEffectVolume);

	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseOption);
	BGMVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedBGMVolume);
	EffectVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedEffectVolume);

	//checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	//BGMVolumSlider->SetValue(UIHandle->GetBGMVolumeByGameInstance());
	//EffectVolumSlider->SetValue(UIHandle->GetEffectVolumeByGameInstance());
}



void UOptionWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	//UIHandle->RemoveCoverFromViewport(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedCloseOption()
{
	//OnClickedCloseWidget(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedBGMVolume(float Value)
{
	//UISubsystem이 존재하는지 체크
	//UISubsystem 혹은 UIHandler에 입력받은 Value로 볼륨 변경 요청.
	
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedBGMVolume(Value);
	//UIHandle->RequestPlayUISound(EUISoundType::Click);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UAudioSubsystem* AudioSubsystem = GameInstance->GetSubsystem<UAudioSubsystem>())
		{
			AudioSubsystem->SetAndApplyMasterVolume(Value);
		}
	}
}

void UOptionWidget::OnClickedEffectVolume(float Value)
{
	//UISubsystem이 존재하는지 체크
	//UISubsystem 혹은 UIHandler에 입력받은 Value로 볼륨 변경 요청.

	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedEffectVolume(Value);
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
}

void UOptionWidget::OpenKeySetting()
{

}