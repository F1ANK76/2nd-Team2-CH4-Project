// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "CooperationWidget.generated.h"

struct FPlayerUIData; //임시 선언// 나중에 Data구조로 따로 빼야됨.

USTRUCT(BlueprintType)
struct FBossUIData  //임시 선언// 나중에 Data구조로 따로 빼야됨.
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString BossName;


	UPROPERTY(BlueprintReadWrite)
	int32 CurrentHP;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxHP;
};


UCLASS()
class ORIGINALSINPRJ_API UCooperationWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	//all stage
	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player1StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player2StateUI;

public:
	void InitPlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2);
	void UpdatePlayerUI(FPlayerUIData* Player1, FPlayerUIData* Player2);
	FPlayerUIData* Player1Data;
	FPlayerUIData* Player2Data;


	//stage2

private:
	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Enemy1StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Enemy2StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Enemy3StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Enemy4StateUI;

public:
	void InitEnemyUI(FPlayerUIData* Enemy1, FPlayerUIData* Enemy2, FPlayerUIData* Enemy3, FPlayerUIData* Enemy4);
	void UpdateEnemyUI(FPlayerUIData* Enemy1, FPlayerUIData* Enemy2, FPlayerUIData* Enemy3, FPlayerUIData* Enemy4);

	FPlayerUIData* Enemy1Data;
	FPlayerUIData* Enemy2Data;
	FPlayerUIData* Enemy3Data;
	FPlayerUIData* Enemy4Data;

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossCurrentHPText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossMaxHPText;

	//stage3
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainHPLine;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShowForwardSlash;




	int32 HPPerLine = 10;
	int32 BossRemaingHPMaxLine = 99;

	// 타이머 변수
	FTimerHandle StageTimerHandle;
	float ElapsedTime = 0.0f;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossModeTimer;

	UFUNCTION()
	void StartStageTimer();

	UFUNCTION()
	void TickStageTimer();

	UFUNCTION()
	void StopStageTimer();

	/// <summary>
	/// ////
	/// </summary>
	/// 
	/// 
	UPROPERTY(meta = (BindWidget))
	class UButton* tempDamage;

	UPROPERTY(meta = (BindWidget))
	class UButton* temp_boss_init;

	UPROPERTY(meta = (BindWidget))
	class UButton* temp_boss_die;

	UPROPERTY(meta = (BindWidget))
	class UButton* StageButton1;

	UPROPERTY(meta = (BindWidget))
	class UButton* StageButton2;

	UPROPERTY(meta = (BindWidget))
	class UButton* StageButton3;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartTimerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StopTimerButton;

	UFUNCTION()
	void damage();

	UFUNCTION()
	void initHP();

	UFUNCTION()
	void dieHP();

	//stage3 BossUI (HP) (timer)

	void InitBossUI(FBossUIData* BossData);
	void UpdateBossUI(FBossUIData* BossData);

	int32 temp_boss_hp = 1000;
	int32 max_boss_hp = 1000;

	void tempUpdateBossUI();
	///


public:
	int32 CurrentStage = 1;
	UFUNCTION()
	void ActiveWidgetbyStage(int32 StageIndex);
	UFUNCTION()
	void ActiveStage1Widget();
	UFUNCTION()
	void ActiveStage2Widget();
	UFUNCTION()
	void ActiveStage3Widget();
	UFUNCTION()
	void DeactivateAllWidgets();

	UPROPERTY()
	TArray<FLinearColor> ProgressBarColors;

	int32 colorindex = 0;


};