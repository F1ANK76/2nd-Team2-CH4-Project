// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/MoveAbility.h"
#include "Player/BaseWitch.h"

const float AMoveAbility::RotateValue = 90;

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
	
	FRotator NewRotation(0, RotateValue, 0);

	if (DirectionValue.X < 0)
	{
		NewRotation *= -1;
	}
	
	ParentWitch->AddMovementInput(GetActorRightVector(), -DirectionValue.X);
	ParentWitch->SetActorRotation(NewRotation);
	ParentWitch->SetWitchState(EWitchStateType::Move);
}
