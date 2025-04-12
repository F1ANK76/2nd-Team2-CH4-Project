// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "BattleWidget.generated.h"

struct FPlayerData; //�ӽ� ����// ���߿� Data������ ���� ���ߵ�.

UCLASS()
class ORIGINALSINPRJ_API UBattleWidget : public UBaseWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	//Ÿ�̸�

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FarmingModeTimer;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeLimitModeTimer;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateFarmingModeTimerUI(float time);

	UFUNCTION(BlueprintCallable)
	void UpdateTimerLimitModeTimerUI(float time);

private:
	//all stage
	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player1StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player2StateUI;

public:
	UFUNCTION(BlueprintCallable)
	void InitPlayerUI(FPlayerData Player1, FPlayerData Player2);
	
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerUI(FPlayerData Player1, FPlayerData Player2);


	UFUNCTION()
	void ActiveFarmingModeWidget();
	UFUNCTION()
	void ActiveTimeLimitModeWidget();


	UFUNCTION()
	void DeactiveFarmingModeWidget();
	UFUNCTION()
	void DeactiveTimeLimitModeWidget();

	FPlayerData* Player1Data;
	FPlayerData* Player2Data;
	
};
