// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/LowerAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"

void ALowerAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::LowerAttackAbility;
}

bool ALowerAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
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
		FVector NewLocation = Buffer.ParentWitch->GetHeadLocation();

		NewLocation.Z += AddedHeight;

		ProjectileObj->SetActorLocation(NewLocation);
		ProjectileObj->ActiveProjectile(Buffer, DelayTime);
	}

	return true;
}

void ALowerAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	if (IsValid(ProjectileObj))
	{
		ProjectileObj->DeactiveProjectile();
	}
}
