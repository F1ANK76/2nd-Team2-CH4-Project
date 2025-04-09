// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "OptionWidget.generated.h"

class UUISubsystem;
class UButton;
class USlider;

UCLASS()
class ORIGINALSINPRJ_API UOptionWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void InitWidget(UUISubsystem* NewUIHandle) override;
	virtual void EndRemoveAnim() override;

private:
	UFUNCTION()
	void OnClickedCloseOption();

	UFUNCTION()
	void OnClickedBGMVolume(float Value);

	UFUNCTION()
	void OnClickedEffectVolume(float Value);

	UFUNCTION()
	void OpenKeySetting();


	//Button Binding
	/*
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OpenKeySettingButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	*/

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;



	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> BGMVolumeSlider;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> EffectVolumeSlider;


	//UI Animation
	/*
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenOptionAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseOptionAnim;
	*/
};
