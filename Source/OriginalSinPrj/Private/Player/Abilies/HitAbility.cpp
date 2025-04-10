// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/HitAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

AHitAbility::AHitAbility() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AHitAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::HitAbility;
}

bool AHitAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s : Init Hit Ability Excute"), *Buffer.ParentWitch->GetName());

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;
	// TODO : Hit Event

	if (bIsHittedFromLeft)
	{

	}
	else
	{

	}

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Hit);
	Buffer.ParentWitch->PlayAnimation(AbilityMontage);

	SetActorTickEnabled(true);

	return true;
}

void AHitAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	/*if (!bCompleteKnock)
	{
		return;
	}*/

	UE_LOG(LogTemp, Warning, TEXT("%s : Init Hit Ability Undo"), *Buffer.ParentWitch->GetName());

	SetActorTickEnabled(false);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Idle);
	Buffer.ParentWitch->StopAnimation(AbilityMontage);
}

bool AHitAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (Buffer.LastAbilities.IsEmpty())
	{
		return true;
	}

	if (!IsValid(Buffer.LastAbilities[0]))
	{
		return true;
	}

	if (Buffer.LastAbilities[0]->GetAbilityType() == EAbilityType::RollAbility)
	{
		return false;
	}

	if (Buffer.LastAbilities[0]->GetAbilityType() == EAbilityType::GuardAbility)
	{
		FVector CauserPos = Buffer.DamageCauser->GetActorLocation();
		HitDirection = CauserPos - Buffer.ParentWitch->GetActorLocation();

		if (HitDirection.Y < 0)
		{
			bIsHittedFromLeft = false;

			if (!Buffer.bIsLeft)
			{
				return false;
			}
		}
		else
		{
			bIsHittedFromLeft = true;

			if (Buffer.bIsLeft)
			{
				return false;
			}
		}
	}

	Buffer.LastAbilities[0]->UndoAbility(Buffer);

	return true;
}

void AHitAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
