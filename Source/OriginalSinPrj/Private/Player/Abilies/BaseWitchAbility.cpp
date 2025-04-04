// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/BaseWitchAbility.h"


ABaseWitchAbility::ABaseWitchAbility()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWitchAbility::InitAbility(ABaseWitch* NewParent)
{
	ParentWitch = NewParent;
}

void ABaseWitchAbility::ExcuteAbility(const FVector2D& DirectionVector)
{

}

void ABaseWitchAbility::UndoAbility()
{

}

void ABaseWitchAbility::BeginPlay()
{
	Super::BeginPlay();
	
}


