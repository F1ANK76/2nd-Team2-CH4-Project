// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/DashAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void ADashAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::DashAttackAbility;
}

bool ADashAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::NormalAttack);

	return true;
}
