// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/LowerAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void ALowerAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::LowerAttackAbility;
}

bool ALowerAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::NormalAttack);

	return true;
}
