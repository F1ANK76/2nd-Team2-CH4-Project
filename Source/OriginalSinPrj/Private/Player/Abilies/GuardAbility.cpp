// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/GuardAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"


void AGuardAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::GuardAbility;
}

bool AGuardAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;
	Buffer.bIsJumpable = false;

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Guard);

	if (Buffer.ComandDirection == EDirectionType::Down)
	{
		Buffer.ParentWitch->PlayAnimation(CrouchGuard);
	}
	else
	{
		Buffer.ParentWitch->PlayAnimation(AbilityMontage);
	}

	return true;
}

void AGuardAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Idle);

	Buffer.ParentWitch->StopAnimation(CrouchGuard);
	Buffer.ParentWitch->StopAnimation(AbilityMontage);

	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;
	Buffer.bIsJumpable = true;
}

bool AGuardAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!IsValid(Buffer.MovementComp))
	{
		return false;
	}

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
