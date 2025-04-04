// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAbilityComponent.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"

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
		MoveAbility = SpawnAbility(MoveAbilityClass);
		MoveAbility->AttachToActor(ParentWitch, FAttachmentTransformRules::KeepWorldTransform);
		MoveAbility->InitAbility(ParentWitch);
	}
	
	CurrentAbility = MoveAbility;
	ExcuteCurrentAbility(Value);
}


void UWitchAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetOwner()))
	{
		ParentWitch = Cast<ABaseWitch>(GetOwner());
	}

	checkf(IsValid(ParentWitch), TEXT("Ability Component : Parent is invalid. Parent == nullptr || Not BaseWitch type"));
}

ABaseWitchAbility* UWitchAbilityComponent::SpawnAbility(UClass* TargetClass)
{
	if (!IsValid(ParentWitch))
	{
		if (IsValid(GetOwner()))
		{
			ParentWitch = Cast<ABaseWitch>(GetOwner());
		}
		else
		{
			return nullptr;
		}
	}

	FActorSpawnParameters SpawnParam;
	FVector SpawnPos = ParentWitch->GetActorLocation();
	FRotator SpawnRot = ParentWitch->GetActorRotation();

	return GetWorld()->SpawnActor<ABaseWitchAbility>(TargetClass, SpawnPos, SpawnRot, SpawnParam);
}

void UWitchAbilityComponent::ExcuteCurrentAbility(const FVector2D& DirectionVector)
{
	if (!IsValid(CurrentAbility))
	{
		return;
	}

	CurrentAbility->ExcuteAbility(DirectionVector);
	AddLastAbilityToArray();
	ActiveTimer();
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



