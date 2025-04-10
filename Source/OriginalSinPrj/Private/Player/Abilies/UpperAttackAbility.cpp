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
	bool ParentResult = Super::ExcuteAbility(Buffer);

	if (!ParentResult)
	{
		return false;
	}

	Buffer.ParentWitch->PlayEffect(EEffectVisibleType::Right);

	return true;
}
