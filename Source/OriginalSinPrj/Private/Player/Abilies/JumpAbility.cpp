// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/JumpAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"

void AJumpAbility::InitAbility()
{
	Super::InitAbility();

	AbilityType = EAbilityType::JumpAbility;
}

bool AJumpAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	ResponseJumped(Buffer.ParentWitch);
	Buffer.ParentWitch->SetWitchState(EWitchStateType::Jump);

	return true;
}

void AJumpAbility::ResponseJumped_Implementation(ABaseWitch* ParentWitch)
{
	if (!IsValid(ParentWitch))
	{
		return;
	}

	if (!ParentWitch->IsLocallyControlled())
	{
		return;
	}

	ParentWitch->Jump();
}

bool AJumpAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!Buffer.bIsJumpable)
	{
		return false;
	}

	return true;
}

//void AJumpAbility::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}
