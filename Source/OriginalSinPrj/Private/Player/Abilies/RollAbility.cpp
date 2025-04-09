// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/RollAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void ARollAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::RollAbility;
}

bool ARollAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}
	
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;
	Buffer.bIsJumpable = false;

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Roll);

	if (Buffer.bIsLeft)
	{
		if (Buffer.ComandDirection == EDirectionType::Left)
		{
			Buffer.ParentWitch->PlayAnimation(AbilityMontage);
		}
		else
		{
			Buffer.ParentWitch->PlayAnimation(BackRoll);
		}
	}
	else
	{
		if (Buffer.ComandDirection == EDirectionType::Right)
		{
			Buffer.ParentWitch->PlayAnimation(AbilityMontage);
		}
		else
		{
			Buffer.ParentWitch->PlayAnimation(BackRoll);
		}
	}

	return true;
}

void ARollAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	if (Buffer.LastAbilities.Num() > 1)
	{
		if (IsValid(Buffer.LastAbilities[1]))
		{
			Buffer.LastAbilities[1]->UndoAbility(Buffer);
		}
	}
}

bool ARollAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!IsValid(Buffer.CurrentAbility))
	{
		return false;
	}

	if (Buffer.LastAbilities[0]->GetAbilityType() != EAbilityType::GuardAbility)
	{
		return false;
	}

	if (Buffer.ComandDirection == EDirectionType::Up)
	{
		return false;
	}
	
	if (Buffer.ComandDirection == EDirectionType::Down)
	{
		return false;
	}
	
	if (Buffer.ComandDirection == EDirectionType::None)
	{
		return false;
	}
	return true;
}
