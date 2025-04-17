// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "LobbyWidget.generated.h"

class UMapSelectWidget;
class UCharacterSelectWidget;
class UGameSettingWidget;
class UButton;
class UOriginalSinPrjGameInstance;
class AWitchController;


UCLASS()
class ORIGINALSINPRJ_API ULobbyWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
    virtual void InitWidget(UUISubsystem* uiSubsystem) override;
    virtual void NativeConstruct() override;

protected:
    // ��/ĳ����/�ɼ� UI ���� ����
    /*UPROPERTY(meta = (BindWidget))
    UMapSelectWidget* MapSelectWidget;*/

    UPROPERTY(meta = (BindWidget))
    UCharacterSelectWidget* CharacterSelectWidget;

    UPROPERTY(meta = (BindWidget))
    UGameSettingWidget* GameSettingWidget;

    // �޴� ��ư
    /*UPROPERTY(meta = (BindWidget))
    UButton* MapSelectButton;*/

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

    // Ŭ�� �̺�Ʈ �Լ�
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

    bool CheckValidOfGameInstance();
    bool CheckValidOfPlayerController();

protected:
    UPROPERTY()
    TObjectPtr<UOriginalSinPrjGameInstance> LocalGameInstance = nullptr;

    UPROPERTY()
    TObjectPtr<AWitchController> OwningPC = nullptr;

    bool bIsReadied = false;
};
