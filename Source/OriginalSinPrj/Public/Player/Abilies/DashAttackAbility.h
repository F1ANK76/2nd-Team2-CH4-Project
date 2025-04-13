// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/AttackAbility.h"
#include "DashAttackAbility.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ADashAttackAbility : public AAttackAbility
{
	GENERATED_BODY()
	
public:
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) override;
};
