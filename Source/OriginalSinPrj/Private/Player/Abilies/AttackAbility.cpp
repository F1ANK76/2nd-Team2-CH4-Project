// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/AttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"

bool AAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Attack);
	Buffer.ParentWitch->PlayAnimation(AbilityMontage);

	return true;
}

void AAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.ParentWitch->StopAnimation(AbilityMontage);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;
}

bool AAttackAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!Buffer.bIsUseable)
	{
		return false;
	}

	return true;
}

void AAttackAbility::SpawnProjectile(FAbilityDataBuffer& Buffer)
{
	if (!IsValid(ProjectileClass))
	{
		return;
	}

	ProjectileObj = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass);
	ProjectileObj->InitProjectile(Buffer.ParentWitch);
}
