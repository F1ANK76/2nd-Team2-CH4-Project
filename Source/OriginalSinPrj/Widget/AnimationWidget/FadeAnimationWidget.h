// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "FadeAnimationWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALSINPRJ_API UFadeAnimationWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUISubsystem* NewUIHandle) override;
	//virtual void PlayRemoveAnim(bool bIsNext, ESceneType SceneType = ESceneType::Title) override;
	virtual void PlayAddAnim() override; //FadeIn
	virtual void EndRemoveAnim() override;
	virtual void EndAddAnim() override;

private:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	bool bIsFadeNext = false;
	//ESceneType FadeSceneType = ESceneType::Title;

};
