// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/ChopAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"

void AChopAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::ChopAttackAbility;
}

bool AChopAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	bool ParentResult = Super::ExcuteAbility(Buffer);

	if (!ParentResult)
	{
		return false;
	}

	if (!IsValid(ProjectileObj))
	{
		SpawnProjectile(Buffer);
	}

	if (IsValid(ProjectileObj))
	{
		FVector NewLocation = Buffer.ParentWitch->GetActorLocation();

		NewLocation.Z -= 90;

		ProjectileObj->SetActorLocation(NewLocation);
		ProjectileObj->ActiveProjectile(Buffer, DelayTime);
	}

	return true;
}

void AChopAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	if (IsValid(ProjectileObj))
	{
		ProjectileObj->DeactiveProjectile();
	}
}

