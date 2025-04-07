// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/LowerAttackAbility.h"
#include "Player/BaseWitch.h"

void ALowerAttackAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::LowerAttackAbility;
}

void ALowerAttackAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
}
