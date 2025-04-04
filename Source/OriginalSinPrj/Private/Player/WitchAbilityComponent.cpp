// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAbilityComponent.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"
#include "GameFramework/CharacterMovementComponent.h"

UWitchAbilityComponent::UWitchAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWitchAbilityComponent::CheckMoveable(const FVector2D& Value)
{
	ParentMovementComp->IsWalkable(FHitResult());
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

	ParentMovementComp = ParentWitch->GetCharacterMovement();
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

	if (!IsValid(TargetClass))
	{
		return nullptr;
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
	LastAbilities.Add(CurrentAbility);
	LastAbilities.Insert(CurrentAbility, 0);
	RemoveOldAbilityFromArray();
}

void UWitchAbilityComponent::RemoveOldAbilityFromArray()
{
	if (LastAbilities.Num() > 5)
	{
		LastAbilities.RemoveAt(5);
	}
}

void UWitchAbilityComponent::ClearLastAbilities()
{
	LastAbilities.Empty();
}



