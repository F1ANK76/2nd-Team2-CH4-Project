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
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	
protected:
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	/*virtual void Tick(float DeltaTime) override;*/
};
