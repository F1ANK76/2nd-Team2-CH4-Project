// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/HitAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void AHitAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::HitAbility;
}

bool AHitAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	// TODO : Hit Event

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Hit);
	Buffer.ParentWitch->PlayAnimation(AbilityMontage);

	return true;
}

bool AHitAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!IsValid(Buffer.CurrentAbility))
	{
		return true;
	}

	if (Buffer.CurrentAbility->GetAbilityType() == EAbilityType::RollAbility)
	{
		return false;
	}

	if (Buffer.CurrentAbility->GetAbilityType() == EAbilityType::GuardAbility)
	{
		FVector CauserPos = Buffer.DamageCauser->GetActorLocation();
		FVector Direction = CauserPos - Buffer.ParentWitch->GetActorLocation();

		if (Direction.Y < 0)
		{
			if (!Buffer.bIsLeft)
			{
				return false;
			}
		}
		else
		{
			if (Buffer.bIsLeft)
			{
				return false;
			}
		}
	}

	Buffer.CurrentAbility->UndoAbility(Buffer);

	return true;
}
