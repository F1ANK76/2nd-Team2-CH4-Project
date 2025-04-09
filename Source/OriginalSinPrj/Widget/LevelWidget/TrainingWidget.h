// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "TrainingWidget.generated.h"


class UButton;


UCLASS()
class ORIGINALSINPRJ_API UTrainingWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUISubsystem* NewUIHandle) override;
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnClickedPlayerSettingButton();


private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> ToTitleButton;

	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OpenPlayerSettingButton;
	

	//임시로 블루프린트에서 구현중...
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SettingWindowSlideInAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SettingWindowSlideOutAnim;
	
	bool bIsSlided = false;
};
