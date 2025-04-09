// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/TauntAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"

void ATauntAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::TauntAbility;
}

bool ATauntAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Taunt);
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;
	Buffer.bIsJumpable = false;
	
	if (Buffer.ComandDirection == EDirectionType::Up)
	{
		Buffer.ParentWitch->PlayAnimation(UpTaunt);
	}
	else if (Buffer.ComandDirection == EDirectionType::Down)
	{
		Buffer.ParentWitch->PlayAnimation(DownTaunt);
	}
	else
	{
		Buffer.ParentWitch->PlayAnimation(AbilityMontage);
	}

	return true;
}

void ATauntAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;
	Buffer.bIsJumpable = true;

	Buffer.ParentWitch->StopAnimation(UpTaunt);
	Buffer.ParentWitch->StopAnimation(DownTaunt);
	Buffer.ParentWitch->StopAnimation(AbilityMontage);
}

bool ATauntAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (Buffer.MovementComp->IsFalling())
	{
		return false;
	}

	if (!Buffer.bIsUseable)
	{
		return false;
	}

	return true;
}
