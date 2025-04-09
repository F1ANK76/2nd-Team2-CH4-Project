// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/NormalAttackAbility.h"
#include "Player/BaseWitch.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Struct/AbilityDataBuffer.h"

void ANormalAttackAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::NormalAttackAbility;
}

bool ANormalAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;
	Buffer.ParentWitch->SetWitchState(EWitchStateType::NormalAttack);
	
	UBoxComponent* Damager = Buffer.ParentWitch->GetDamager(EDirectionType::Right);

	if (!IsValid(Damager))
	{
		return false;
	}

	Buffer.ParentWitch->PlayAnimation(AbilityMontage);

	TArray<AActor*> HittedActors;
	Damager->GetOverlappingActors(HittedActors);

	if (HittedActors.IsEmpty())
	{
		return false;
	}

	for (AActor* HittedActor : HittedActors)
	{
		if (HittedActor == Buffer.ParentWitch)
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(HittedActor, Damage, Buffer.ParentWitch->GetController(), Buffer. ParentWitch, UDamageType::StaticClass());
	}

	return true;
}

void ANormalAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.ParentWitch->StopAnimation(AbilityMontage);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;
}

bool ANormalAttackAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!Buffer.bIsUseable)
	{
		return false;
	}

	return true;
}
