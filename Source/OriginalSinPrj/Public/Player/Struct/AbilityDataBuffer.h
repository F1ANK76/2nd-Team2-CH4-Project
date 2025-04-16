// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/WitchTypes.h"
#include "AbilityDataBuffer.generated.h"

class ABaseWitchAbility;
class ABaseWitch;
class UCharacterMovementComponent;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FAbilityDataBuffer
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	EDirectionType ComandDirection = EDirectionType::None;

	UPROPERTY()
	FVector2D MoveValueVector = FVector2D::ZeroVector;

	UPROPERTY()
	bool bIsLeft = false;
	
	UPROPERTY()
	bool bIsMoveable = true;

	UPROPERTY()
	bool bIsUseable = true;

	UPROPERTY()
	bool bIsJumpable = true;

	UPROPERTY()
	bool bIsInAir = false;

	UPROPERTY()
	float KnockGuage = 0.0f;

	UPROPERTY()
	float AddedGuage = 0.0f;

	UPROPERTY()
	float AddedDamage = 0.0f;

	UPROPERTY()
	float MaxMana = 5;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentMana = 5;

	UPROPERTY()
	float NeedMana = 0;

	UPROPERTY()
	TObjectPtr<AActor> DamageCauser = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> CurrentAbility = nullptr;

	UPROPERTY()
	TArray<ABaseWitchAbility*> LastAbilities;

	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComp = nullptr;
};
