// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "BattleWidget.generated.h"

struct FPlayerUIData; //임시 선언// 나중에 Data구조로 따로 빼야됨.

UCLASS()
class ORIGINALSINPRJ_API UBattleWidget : public UBaseWidget
{
	GENERATED_BODY()

	//타이머

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
	
};
