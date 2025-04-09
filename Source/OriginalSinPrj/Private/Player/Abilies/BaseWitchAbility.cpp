// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"


ABaseWitchAbility::ABaseWitchAbility()
{
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);
	//SetReplicateMovement(true);
}

const EAbilityType& ABaseWitchAbility::GetAbilityType() const
{
	return AbilityType;
}

void ABaseWitchAbility::BeginPlay()
{
	Super::BeginPlay();
	
}


