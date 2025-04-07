// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "JumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALSINPRJ_API AJumpAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	virtual void InitAbility(ABaseWitch* NewParent) override;
	virtual void ExcuteAbility(const FVector2D& DirectionValue) override;
	
	
};
