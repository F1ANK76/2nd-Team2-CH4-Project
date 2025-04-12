// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/UppercutAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"


void AUppercutAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::UppercutAttackAbility;
}

bool AUppercutAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
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

		NewLocation.Z -= 80;

		if (Buffer.bIsLeft)
		{
			NewLocation.Y -= 200;
		}
		else
		{
			NewLocation.Y += 200;
		}

		ProjectileObj->SetActorLocation(NewLocation);
		ProjectileObj->ActiveProjectile(Buffer, DelayTime);
	}

	return true;
}

void AUppercutAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	if (IsValid(ProjectileObj))
	{
		ProjectileObj->DeactiveProjectile();
	}
}
