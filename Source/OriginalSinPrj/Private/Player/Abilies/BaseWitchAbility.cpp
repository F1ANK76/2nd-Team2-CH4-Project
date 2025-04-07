// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"


ABaseWitchAbility::ABaseWitchAbility()
{
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);
	//SetReplicateMovement(true);
}

void ABaseWitchAbility::InitAbility(ABaseWitch* NewParent)
{
	ParentWitch = NewParent;
}

void ABaseWitchAbility::ExcuteAbility(const FVector2D& DirectionVector)
{
	if (IsValid(AbilityMontage))
	{
		ParentWitch->PlayAnimation(AbilityMontage);
	}
}

void ABaseWitchAbility::UndoAbility()
{

}

const EAbilityType& ABaseWitchAbility::GetAbilityType() const
{
	return AbilityType;
}

void ABaseWitchAbility::BeginPlay()
{
	Super::BeginPlay();
	
}


