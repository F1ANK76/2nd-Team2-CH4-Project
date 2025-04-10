// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "BattleWidget.generated.h"

struct FPlayerUIData; //�ӽ� ����// ���߿� Data������ ���� ���ߵ�.

UCLASS()
class ORIGINALSINPRJ_API UBattleWidget : public UBaseWidget
{
	GENERATED_BODY()

	//Ÿ�̸�

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
