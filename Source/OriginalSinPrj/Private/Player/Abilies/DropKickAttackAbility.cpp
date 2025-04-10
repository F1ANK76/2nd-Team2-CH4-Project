// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/DropKickAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/GroundProjectile.h"

void ADropKickAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::DropkickAttackAbility;
}

bool ADropKickAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
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
		FRotator NewRotation = Buffer.ParentWitch->GetActorRotation();
		FVector NewLocation = Buffer.ParentWitch->GetActorLocation();

		if (Buffer.bIsLeft)
		{
			NewRotation.Yaw = -90;
		}
		else
		{
			NewRotation.Yaw = 90;
		}
		
		NewLocation.Z = 0;

		ProjectileObj->SetActorLocation(NewLocation);
		ProjectileObj->SetActorRotation(NewRotation);


		ProjectileObj->ActiveProjectile(Buffer, DelayTime);
	}

	return true;
}

void ADropKickAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	if (IsValid(ProjectileObj))
	{
		ProjectileObj->DeactiveProjectile();
	}
}
