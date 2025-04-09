// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"


class UUIHandle;
enum EWidgetType;
enum ESceneType;

UCLASS()
class ORIGINALSINPRJ_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
	
public:
	virtual void InitWidget(UUIHandle* uiHandle);
	virtual void Action();
	virtual void Update();
	virtual void PlayUIOpenAnimation();
	virtual void PlayUICloseAnimation();
	virtual void PlayUIOpenAnimation(UWidgetAnimation* UIOpenAnimation);
	virtual void PlayUICloseAnimation(UWidgetAnimation* UIcloseAnimation);
	
	virtual void PlayAddAnim();
	virtual void PlayRemoveAnim();
	//virtual void PlayRemoveAnim(bool bIsNext, ESceneType SceneType = ESceneType::Title);

protected:
	UFUNCTION()
	virtual void StartAddAnim();

	UFUNCTION()
	virtual void EndAddAnim();

	UFUNCTION()
	virtual void StartRemoveAnim();

	UFUNCTION()
	virtual void EndRemoveAnim();

	UFUNCTION()
	virtual void OnClickedMoveNext();

	UFUNCTION()
	virtual void OnClickedMoveTitle();

	UFUNCTION()
	virtual void OnClickedMoveSingleMode();

	UFUNCTION()
	virtual void OnClickedMoveMultiMode();

	UFUNCTION()
	virtual void OnClickedMoveTrainingMode();

	UFUNCTION()
	virtual void OnClickedQuitGame();

	UFUNCTION()
	virtual void OnClick_MoveLevel();
	
	virtual void OnClickedOpenWidget(const EWidgetType WidgetType);
	virtual void OnClickedCloseWidget(const EWidgetType WidgetType);

protected:
	FWidgetAnimationDynamicEvent StartAddDelegate;
	FWidgetAnimationDynamicEvent EndAddDelegate;
	FWidgetAnimationDynamicEvent StartRemoveDelegate;
	FWidgetAnimationDynamicEvent EndRemoveDelegate;

	TObjectPtr<UWidgetAnimation> OpenAnimation;
	TObjectPtr<UWidgetAnimation> CloseAnimation;

	TObjectPtr<UUIHandle> UIHandle;
	//ESceneType MoveTargetScene = ESceneType::Title;
	bool bIsPlaying = false;

	
};
