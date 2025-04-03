// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAbilityComponent.h"
#include "Player/Abilies/BaseWitchAbility.h"

UWitchAbilityComponent::UWitchAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWitchAbilityComponent::CheckMoveable(const FVector2D& Value)
{
	if (!bIsMoveable)
	{
		return;
	}

	if (!IsValid(MoveAbility))
	{
		
	}
}


void UWitchAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UWitchAbilityComponent::ActiveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BufferTimer, this, &ThisClass::ClearLastAbilities, BufferActiveTime, false);
}

void UWitchAbilityComponent::AddLastAbilityToArray()
{

}

void UWitchAbilityComponent::RemoveOldAbilityFromArray()
{

}

void UWitchAbilityComponent::ClearLastAbilities()
{

}



