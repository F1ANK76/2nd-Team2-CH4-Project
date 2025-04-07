// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"


class UUIHandle;

UCLASS()
class ORIGINALSINPRJ_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // UI 표시 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Show();

    // UI 숨김 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Hide();

protected:
    virtual void NativeConstruct() override;

    // 애니메이션을 위한 변수 (Blueprint에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetAnimation* FadeInAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetAnimation* FadeOutAnimation;
	
	
public:
	virtual void InitWidget(UUIHandle* uiHandle);
	virtual void Action();
	virtual void Update();
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
	virtual void OnClickedMoveMain();

	UFUNCTION()
	virtual void OnClickedMoveTitle();

	UFUNCTION()
	virtual void OnClickedQuitGame();

	//virtual void OnClickedOpenWidget(const EWidgetType WidgetType);
	//virtual void OnClickedCloseWidget(const EWidgetType WidgetType);

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
