// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/WitchTypes.h"
#include "GameSettingData.generated.h"


USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FGameSettingData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsHost = false;

	UPROPERTY()
	bool bPlayFarming = true;

	UPROPERTY()
	bool bApplyBuff = true;

	UPROPERTY()
	bool bIsCustomLife = false;

	UPROPERTY()
	bool bIsTimeLimitMode = false;

	UPROPERTY()
	int32 CustomLifePoint = 1;

	UPROPERTY()
	float CustomTime = 60.0f;

	UPROPERTY()
	ECharacterType SelectedCharacterType = ECharacterType::Hand;

	UPROPERTY()
	int32 PlayerLevel = 0;
};
