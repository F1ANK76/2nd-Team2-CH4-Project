// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/NormalAttackAbility.h"
#include "Player/BaseWitch.h"

void ANormalAttackAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::NormalAttackAbility;
}

void ANormalAttackAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
}
