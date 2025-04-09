// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/UpperAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void AUpperAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::LowerAttackAbility;
}

bool AUpperAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::NormalAttack);

	return true;
}
