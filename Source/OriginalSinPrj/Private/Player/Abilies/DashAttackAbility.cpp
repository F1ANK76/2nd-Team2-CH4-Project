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
	bool ParentResult = Super::ExcuteAbility(Buffer);

	if (!ParentResult)
	{
		return false;
	}

	Buffer.ParentWitch->PlayEffect(EEffectVisibleType::Right);

	return true;
}
