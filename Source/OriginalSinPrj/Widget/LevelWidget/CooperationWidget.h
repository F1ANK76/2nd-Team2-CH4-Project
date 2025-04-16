// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "CooperationWidget.generated.h"

struct FPlayerData; 

USTRUCT(BlueprintType)
struct FBossUIData 
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

public:
	virtual void InitWidget(UUISubsystem* uiSubsystem) override;

protected:
	virtual void NativeConstruct() override;

private:
	//all stage
	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player1StateUI;

	UPROPERTY(meta = (BindWidget))
	class UPlayerStateWidget* Player2StateUI;

public:
	void InitPlayerUI(FPlayerData* Player1, FPlayerData* Player2);
	void UpdatePlayerUI(FPlayerData* Player1, FPlayerData* Player2);
	FPlayerData* Player1Data;
	FPlayerData* Player2Data;


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
	void InitEnemyUI(FPlayerData* Enemy1, FPlayerData* Enemy2, FPlayerData* Enemy3, FPlayerData* Enemy4);
	void UpdateEnemyUI(FPlayerData* Enemy1, FPlayerData* Enemy2, FPlayerData* Enemy3, FPlayerData* Enemy4);

	FPlayerData* Enemy1Data;
	FPlayerData* Enemy2Data;
	FPlayerData* Enemy3Data;
	FPlayerData* Enemy4Data;

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



	FTimerHandle StageTimerHandle;
	float ElapsedTime = 0.0f;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossModeTimer;

	//stage3 BossUI (HP) (timer)

	void InitBossUI(const FBossUIData& BossData);
	void UpdateBossUI(const FBossUIData& BossData);

	int32 temp_boss_hp = 1000;
	int32 max_boss_hp = 1000;
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

	UFUNCTION()
	void UpdateBossTimer(float time);

	UPROPERTY()
	TArray<FLinearColor> ProgressBarColors;

	int32 colorindex = 0;


};