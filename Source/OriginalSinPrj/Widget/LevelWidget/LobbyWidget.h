// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "LobbyWidget.generated.h"

class UMapSelectWidget;
class UCharacterSelectWidget;
class UGameSettingWidget;
class UButton;

UCLASS()
class ORIGINALSINPRJ_API ULobbyWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // 맵/캐릭터/옵션 UI 위젯 참조
    UPROPERTY(meta = (BindWidget))
    UMapSelectWidget* MapSelectWidget;

    UPROPERTY(meta = (BindWidget))
    UCharacterSelectWidget* CharacterSelectWidget;

    UPROPERTY(meta = (BindWidget))
    UGameSettingWidget* GameSettingWidget;

    // 메뉴 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* MapSelectButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CharacterSelectButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GameSettingButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ReadyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GameStartButton;

    // 클릭 이벤트 함수
    UFUNCTION()
    void OnClickMapSelect();

    UFUNCTION()
    void OnClickCharacterSelect();

    UFUNCTION()
    void OnClickGameSetting();

    UFUNCTION()
    void OnClickReady();

    UFUNCTION()
    void OnClickGameStart();
};
