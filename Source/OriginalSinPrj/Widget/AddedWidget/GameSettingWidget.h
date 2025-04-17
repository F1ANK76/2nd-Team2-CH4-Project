// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "GameSettingWidget.generated.h"

class UCheckBox;
class UButton;
class UTextBlock;

UCLASS()
class ORIGINALSINPRJ_API UGameSettingWidget : public UBaseWidget
{
	GENERATED_BODY()
	
	//�Ĺָ�� ���� UI
	//�ð����� ���� UI
	//���� ���� ���� UI

public:
    virtual void InitWidget(UUISubsystem* uiSubsystem) override;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* FarmingModeCheckBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* BuffModeCheckBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* LifePointsModeCheckBox;
    
    UPROPERTY(meta = (BindWidget))
    UCheckBox* TimeLimitModeCheckBox;

    UFUNCTION()
    void OnFarmingCheckBoxChanged(bool bIsChecked);

    UFUNCTION()
    void OnLifePointCheckBoxChanged(bool bIsChecked);

    UFUNCTION()
    void OnBuffCheckBoxChanged(bool bIsChecked);

    UFUNCTION()
    void OnTimeLimitCheckBoxChanged(bool bIsChecked);

    bool bIsLifePointModeChecked;
    bool bIsFarmingModeChecked;
    bool bIsBuffModeChecked;
    bool bIsTimeLimitModeChecked;

    //�ð� ����
    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox1;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox2;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox3;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox4;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* CheckBox5;

    UFUNCTION()
    void OnCheckBox1Changed(bool bIsChecked);

    UFUNCTION()
    void OnCheckBox2Changed(bool bIsChecked);

    UFUNCTION()
    void OnCheckBox3Changed(bool bIsChecked);

    UFUNCTION()
    void OnCheckBox4Changed(bool bIsChecked);

    UFUNCTION()
    void OnCheckBox5Changed(bool bIsChecked);

    UFUNCTION()
    void OnClickedCloseButton();

    void UncheckAllExcept(UCheckBox* CheckedBox);

    //����������Ʈ

    UPROPERTY(meta = (BindWidget))
    UButton* LifePointsIncreaseButton;
    
    UPROPERTY(meta = (BindWidget))
    UButton* LifePointsDecreaseButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LifePoints;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> CloseButton;


    UFUNCTION()
    void OnClickedIncreaseLifePoints();

    UFUNCTION()
    void OnClickedDecreaseLifePoints();

    void UpdateLifePoints();

    int32 LifePoint = 1;

public:
    int32 SelectedTimeIndex = -1; // ���õ� �ε����� ����
};
