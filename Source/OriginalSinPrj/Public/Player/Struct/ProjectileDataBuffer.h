// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/WitchTypes.h"
#include "ProjectileDataBuffer.generated.h"

class ABaseWitch;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FProjectileDataBuffer
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	UPROPERTY()
	FVector MoveDirection = FVector::ZeroVector;

	UPROPERTY()
	float MoveSpeed = 0.0f;

	UPROPERTY()
	float AttackDamage = 0.0f;

	UPROPERTY()
	float MoveDelay = 0.0f;

	UPROPERTY()
	float AttackDelay = 0.0f;

	UPROPERTY()
	float VisibleDelay = 0.0f;

	UPROPERTY()
	float DeactiveDelay = 0.0f;
};
