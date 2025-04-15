// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"

#include "../GameInstance/AudioSubsystem.h"

void UOptionWidget::InitWidget(UUISubsystem* uiSubsystem)
{
	Super::InitWidget(uiSubsystem);

	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseOption);
	BGMVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedBGMVolume);
	EffectVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedEffectVolume);
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
	//UISubsystem�� �����ϴ��� üũ
	//UISubsystem Ȥ�� UIHandler�� �Է¹��� Value�� ���� ���� ��û.
	
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
	//UISubsystem�� �����ϴ��� üũ
	//UISubsystem Ȥ�� UIHandler�� �Է¹��� Value�� ���� ���� ��û.

	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedEffectVolume(Value);
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
}

void UOptionWidget::OpenKeySetting()
{

}