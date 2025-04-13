// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/MoveAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"


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

		if (Buffer.MovementComp->IsFalling())
		{
			return false;
		}

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
	bool bIsOldLeft = Buffer.bIsLeft;
	float MoveValue = Buffer.MoveValueVector.X;

	if (Buffer.MoveValueVector.X < 0)
	{
		Buffer.bIsLeft = false;
		MoveValue *= -1;
	}
	else if (Buffer.MoveValueVector.X > 0)
	{
		Buffer.bIsLeft = true;
	}

	if (Buffer.bIsLeft != bIsOldLeft)
	{
		//Buffer.ParentWitch->AddControllerYawInput(RotateValue);
		ResponseRotated(Buffer.ParentWitch, true);
	}

	//Buffer.ParentWitch->AddMovementInput(GetActorRightVector(), MoveValue);
	ResponseMoved(Buffer.ParentWitch, MoveValue);

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

void AMoveAbility::ResponseRotated_Implementation(APawn* ParentWitch, bool bIsRotate)
{
	if (!IsValid(ParentWitch))
	{
		return;
	}

	/*if (!ParentWitch->IsLocallyControlled())
	{
		return;
	}*/

	if (bIsRotate)
	{
		ParentWitch->AddControllerYawInput(RotateValue);
	}
}

void AMoveAbility::ResponseMoved_Implementation(APawn* ParentWitch, float MoveValue)
{
	if (!IsValid(ParentWitch))
	{
		return;
	}

	/*if (!ParentWitch->IsLocallyControlled())
	{
		return;
	}*/

	ParentWitch->AddMovementInput(GetActorRightVector(), MoveValue);
}

