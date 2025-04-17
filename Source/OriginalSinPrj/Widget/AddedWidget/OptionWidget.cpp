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

	if (CheckValidOfAudioSubsystem())
	{
		BGMVolumeSlider->SetValue(AudioSubsystem->GetBgmVolume());
		EffectVolumeSlider->SetValue(AudioSubsystem->GetEffectVolume());
	}
}

void UOptionWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	//UIHandle->RemoveCoverFromViewport(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedCloseOption()
{
	PlayUIEffectSound(EUISfxSoundType::Close);
	//OnClickedCloseWidget(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedBGMVolume(float Value)
{
	PlayUIEffectSound(EUISfxSoundType::Click);
	//UISubsystem�� �����ϴ��� üũ
	//UISubsystem Ȥ�� UIHandler�� �Է¹��� Value�� ���� ���� ��û.
	
	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedBGMVolume(Value);
	//UIHandle->RequestPlayUISound(EUISoundType::Click);

	if (CheckValidOfAudioSubsystem())
	{
		AudioSubsystem->SetBgmVolume(Value);
	}
}

void UOptionWidget::OnClickedEffectVolume(float Value)
{
	PlayUIEffectSound(EUISfxSoundType::Click);
	if (CheckValidOfAudioSubsystem())
	{
		AudioSubsystem->SetEffectVolume(Value);
	}
	//UISubsystem�� �����ϴ��� üũ
	//UISubsystem Ȥ�� UIHandler�� �Է¹��� Value�� ���� ���� ��û.

	//checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	//UIHandle->ClickedEffectVolume(Value);
	//UIHandle->RequestPlayUISound(EUISoundType::Click);
}

void UOptionWidget::OpenKeySetting()
{
	PlayUIEffectSound(EUISfxSoundType::Click);
}

bool UOptionWidget::CheckValidOfAudioSubsystem()
{
	if (IsValid(AudioSubsystem))
	{
		return true;
	}

	if (!IsValid(GetGameInstance()))
	{
		return false;
	}

	AudioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

	if (!IsValid(AudioSubsystem))
	{
		return false;
	}

	return true;
}
