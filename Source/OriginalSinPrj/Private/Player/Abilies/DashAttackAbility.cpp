// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/DashAttackAbility.h"
#include "Player/BaseWitch.h"

void ADashAttackAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::DashAttackAbility;
}

void ADashAttackAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
}
