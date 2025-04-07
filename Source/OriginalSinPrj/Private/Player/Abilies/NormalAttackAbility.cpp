// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/NormalAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

void ANormalAttackAbility::InitAbility(ABaseWitch* NewParent)
{
	Super::InitAbility(NewParent);

	AbilityType = EAbilityType::NormalAttackAbility;
}

void ANormalAttackAbility::ExcuteAbility(const FVector2D& DirectionValue)
{
	Super::ExcuteAbility(DirectionValue);

	ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
	
	UBoxComponent* Damager = ParentWitch->GetDamager(EDirectionType::Right);

	if (!IsValid(Damager))
	{
		return;
	}

	TArray<AActor*> HittedActors;
	Damager->GetOverlappingActors(HittedActors);

	if (HittedActors.IsEmpty())
	{
		return;
	}

	for (AActor* HittedActor : HittedActors)
	{
		if (HittedActor == ParentWitch)
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(HittedActor, Damage, ParentWitch->GetController(), ParentWitch, UDamageType::StaticClass());
	}
}
