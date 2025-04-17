// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffDataBuffer.generated.h"

class ABaseWitch;

USTRUCT()
struct ORIGINALSINPRJ_API FBuffDataBuffer
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	float AddedMana = 0.1f;

	UPROPERTY()
	float AddedKnockGauge = 0.0f;

	UPROPERTY()
	float AddedAttackRange = 1.0f;

	UPROPERTY()
	float AddedAttackSpeed = 1.0f;

	UPROPERTY()
	float AddedGuardPoint = 0.0f;

	UPROPERTY()
	float AddedAvoidRate = 0.0f;
};
