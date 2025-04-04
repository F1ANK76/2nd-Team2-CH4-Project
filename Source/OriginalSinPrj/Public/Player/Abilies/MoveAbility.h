// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "MoveAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALSINPRJ_API AMoveAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	virtual void ExcuteAbility(const FVector2D& DirectionValue) override;
	
	
};
