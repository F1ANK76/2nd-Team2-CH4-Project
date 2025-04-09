// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/MoveAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

const float AMoveAbility::RotateValue = 360;

void AMoveAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::MoveAbility;
}

bool AMoveAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	ExcuteComand(Buffer);

	if (CheckExcuteable(Buffer))
	{
		ExcuteMove(Buffer);
		return true;
	}

	return false;
}

bool AMoveAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!Buffer.bIsMoveable)
	{
		return false;
	}

	if (FMath::IsNearlyZero(Buffer.MoveValueVector.X))
	{
		return false;
	}

	return true;
}

void AMoveAbility::ExcuteMove(FAbilityDataBuffer& Buffer)
{
	if (Buffer.MoveValueVector.X < 0)
	{
		if (Buffer.bIsLeft)
		{
			Buffer.bIsLeft = false;
			Buffer.ParentWitch->AddControllerYawInput(RotateValue);
		}

		Buffer.ParentWitch->AddMovementInput(GetActorRightVector(), -Buffer.MoveValueVector.X);
	}
	else if (Buffer.MoveValueVector.X > 0)
	{
		if (!Buffer.bIsLeft)
		{
			Buffer.bIsLeft = true;
			Buffer.ParentWitch->AddControllerYawInput(RotateValue);
		}

		Buffer.ParentWitch->AddMovementInput(GetActorRightVector(), Buffer.MoveValueVector.X);
	}

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Move);
}

void AMoveAbility::ExcuteComand(FAbilityDataBuffer& Buffer)
{
	if (FMath::IsNearlyZero(Buffer.MoveValueVector.X))
	{
		if (FMath::IsNearlyZero(Buffer.MoveValueVector.Y))
		{
			Buffer.ComandDirection = EDirectionType::None;
			return;
		}

		if (Buffer.MoveValueVector.Y > 0)
		{
			Buffer.ComandDirection = EDirectionType::Up;
		}
		else if (Buffer.MoveValueVector.Y < 0)
		{
			Buffer.ComandDirection = EDirectionType::Down;
		}
	}
	else
	{
		if (Buffer.MoveValueVector.X > 0)
		{
			Buffer.ComandDirection = EDirectionType::Left;
		}
		else if (Buffer.MoveValueVector.X < 0)
		{
			Buffer.ComandDirection = EDirectionType::Right;
		}
	}
}
