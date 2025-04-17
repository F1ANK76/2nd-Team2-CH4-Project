// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "BaseWidget.generated.h"

class UUISubsystem;

UCLASS()
class ORIGINALSINPRJ_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
	void PlayUIEffectSound(EUISfxSoundType SoundType);
	
public:
	virtual void InitWidget(UUISubsystem* uiHandle);

	virtual void Action();
	virtual void Update();
	virtual void PlayUIOpenAnimation();
	virtual void PlayUICloseAnimation();
	virtual void PlayUIOpenAnimation(UWidgetAnimation* UIOpenAnimation);
	virtual void PlayUICloseAnimation(UWidgetAnimation* UIcloseAnimation);

	virtual void PlayAddAnim();
	virtual void PlayRemoveAnim();
	//virtual void PlayRemoveAnim(bool bIsNext, ESceneType SceneType = ESceneType::Title);

	void SetWidgetVisibility(bool bIsVisible); // test

public:
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
	virtual void OnClickedMoveMultiLobbyMode();

	UFUNCTION()
	virtual void OnClickedMoveTrainingMode();

	UFUNCTION()
	virtual void OnClickedQuitGame();

	UFUNCTION()
	virtual void OnClick_MoveLevel();
	
	UFUNCTION()
	virtual void OnClickedOptionButton();

	virtual void OnClickedOpenWidget(const EAddWidgetType WidgetType);
	virtual void OnClickedCloseWidget(const EAddWidgetType WidgetType);

protected:
	FWidgetAnimationDynamicEvent StartAddDelegate;
	FWidgetAnimationDynamicEvent EndAddDelegate;
	FWidgetAnimationDynamicEvent StartRemoveDelegate;
	FWidgetAnimationDynamicEvent EndRemoveDelegate;

	TObjectPtr<UWidgetAnimation> OpenAnimation;
	TObjectPtr<UWidgetAnimation> CloseAnimation;

	TObjectPtr<UUISubsystem> UIHandle;
	//ESceneType MoveTargetScene = ESceneType::Title;
	bool bIsPlaying = false;

	
};
