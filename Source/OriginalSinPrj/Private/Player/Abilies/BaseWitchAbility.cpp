// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"
#include "GameState/BaseGameState.h"

ABaseWitchAbility::ABaseWitchAbility()
{
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);
}

const EAbilityType& ABaseWitchAbility::GetAbilityType() const
{
	return AbilityType;
}

void ABaseWitchAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseWitchAbility::CheckValidOfGameState()
{
	if (!HasAuthority())
	{
		return false;
	}

	if (IsValid(BaseGameState))
	{
		return true;
	}

	if (!IsValid(GetWorld()->GetGameState()))
	{
		return false;
	}

	BaseGameState = GetWorld()->GetGameState<ABaseGameState>();

	if (!IsValid(BaseGameState))
	{
		return false;
	}

	return true;
}


