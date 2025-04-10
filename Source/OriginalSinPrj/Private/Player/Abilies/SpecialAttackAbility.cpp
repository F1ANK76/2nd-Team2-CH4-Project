// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/SpecialAttackAbility.h"

void ASpecialAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::SpecialAttackAbility;
}
