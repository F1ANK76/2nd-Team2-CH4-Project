// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeAnimationWidget.h"

void UFadeAnimationWidget::InitWidget(UUISubsystem* NewUIHandle)
{
	OpenAnimation = FadeInAnim;
	CloseAnimation = FadeOutAnim;
	Super::InitWidget(NewUIHandle);
}

/*
void UFadeAnimationWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	bIsFadeNext = bIsNext;
	FadeSceneType = SceneType;
	//Super::PlayRemoveAnim(bIsNext, SceneType);
}
*/

void UFadeAnimationWidget::PlayAddAnim()
{
	PlayAnimation(FadeInAnim);
}

void UFadeAnimationWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	//UIHandle->RequestMoveSceneByFade(bIsFadeNext, FadeSceneType);
}

void UFadeAnimationWidget::EndAddAnim()
{
	Super::EndAddAnim();

	//UIHandle->RemoveCoverFromViewport(EWidgetType::FadeWidget);
}
