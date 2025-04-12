// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/NormalAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void ANormalAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::NormalAttackAbility;
}

bool ANormalAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	bool ParentResult = Super::ExcuteAbility(Buffer);

	if (!ParentResult)
	{
		return false;
	}

	Buffer.ParentWitch->PlayEffect(EEffectVisibleType::Right);
	Buffer.ParentWitch->PlayMelleAttack(EEffectVisibleType::Right, DamageValue);

	return true;
}

void ANormalAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.ParentWitch->StopMelleAttack();
	Buffer.ParentWitch->StopEffect();
}
