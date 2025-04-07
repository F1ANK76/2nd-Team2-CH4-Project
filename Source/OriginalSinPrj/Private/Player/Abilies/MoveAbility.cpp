// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/MoveAbility.h"
#include "Player/BaseWitch.h"

const float AMoveAbility::RotateValue = 360;

void AMoveAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::MoveAbility;
}

void AMoveAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	if (FMath::IsNearlyZero(DirectionValue.X))
	{
		return;
	}

	if (DirectionValue.X < 0)
	{
		if (bIsLeft)
		{
			bIsLeft = false;
			ParentWitch->AddControllerYawInput(RotateValue);
		}

		ParentWitch->AddMovementInput(GetActorRightVector(), -DirectionValue.X);
	}
	else if (DirectionValue.X > 0)
	{
		if (!bIsLeft)
		{
			bIsLeft = true;
			ParentWitch->AddControllerYawInput(RotateValue);
		}

		ParentWitch->AddMovementInput(GetActorRightVector(), DirectionValue.X);
	}

	ParentWitch->SetWitchState(EWitchStateType::Move);
}
