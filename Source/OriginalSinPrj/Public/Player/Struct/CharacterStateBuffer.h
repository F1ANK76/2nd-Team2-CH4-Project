// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateBuffer.generated.h"


USTRUCT()
struct ORIGINALSINPRJ_API FCharacterStateBuffer
{
	GENERATED_BODY()

public:
    UPROPERTY()
    int32 PlayerLevel = 0;

    UPROPERTY()
    int32 LifePoint = 0;

    UPROPERTY()
    float CurrentHP = 100;

    UPROPERTY()
    float MaxHP = 100;

    UPROPERTY()
    float CurrentMana = 0;

    UPROPERTY()
    float MaxMana = 0;

    UPROPERTY()
    float AirbornePercent = 0;

    UPROPERTY()
    float CurrentEXP = 0;

    UPROPERTY()
    float MaxEXP = 100;

	
};
