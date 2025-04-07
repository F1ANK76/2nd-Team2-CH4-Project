// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/JumpAbility.h"
#include "Player/BaseWitch.h"

void AJumpAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::JumpAbility;
}

void AJumpAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->Jump();
	ParentWitch->SetWitchState(EWitchStateType::Jump);
}
