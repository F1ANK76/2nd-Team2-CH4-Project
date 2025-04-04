// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/MoveAbility.h"
#include "Player/BaseWitch.h"

void AMoveAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	if (FMath::IsNearlyZero(DirectionValue.X))
	{
		return;
	}

	ParentWitch->AddMovementInput(GetActorRightVector(), -DirectionValue.X);
}
