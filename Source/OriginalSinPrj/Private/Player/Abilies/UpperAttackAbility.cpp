// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/UpperAttackAbility.h"
#include "Player/BaseWitch.h"

void AUpperAttackAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::LowerAttackAbility;
}

void AUpperAttackAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
}
