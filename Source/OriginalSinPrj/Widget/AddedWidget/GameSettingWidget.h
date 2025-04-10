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
	
	//파밍모드 설정 UI
	//시간제한 설정 UI
	//버프 여부 설정 UI


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

    //시간 선택
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

    void UncheckAllExcept(UCheckBox* CheckedBox);

    //라이프포인트

    UPROPERTY(meta = (BindWidget))
    UButton* LifePointsIncreaseButton;
    
    UPROPERTY(meta = (BindWidget))
    UButton* LifePointsDecreaseButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LifePoints;


    UFUNCTION()
    void OnClickedIncreaseLifePoints();

    UFUNCTION()
    void OnClickedDecreaseLifePoints();

    void UpdateLifePoints();

    int32 LifePoint = 1;

public:
    int32 SelectedTimeIndex = -1; // 선택된 인덱스를 저장
};
