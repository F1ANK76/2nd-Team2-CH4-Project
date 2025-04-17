// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "OriginalSinPrj/Widget/AddedWidget/PlayerStateWidget.h"
#include "BattleWidget.generated.h"

struct FPlayerData;

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
	void InitPlayerUI(FPlayerData* Player1, FPlayerData* Player2);
	
	void UpdatePlayerUI(FPlayerData* Player1, FPlayerData* Player2);


	UFUNCTION()
	void ActiveFarmingModeWidget();
	UFUNCTION()
	void ActiveTimeLimitModeWidget();


	UFUNCTION()
	void DeactiveFarmingModeWidget();
	UFUNCTION()
	void DeactiveTimeLimitModeWidget();
	
	UFUNCTION()
	void ActivePlayerWidget();

	UFUNCTION()
	void ActiveEnemyWidget();

	UFUNCTION()
	void DeactiveEnemyWidget();


	FPlayerData* Player1Data;
	FPlayerData* Player2Data;
	
};
