// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/HitAbility.h"
#include "Player/BaseWitch.h"

void AHitAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::HitAbility;
}

void AHitAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::Hit);
	ParentWitch->PlayAnimation(AbilityMontage);
}
