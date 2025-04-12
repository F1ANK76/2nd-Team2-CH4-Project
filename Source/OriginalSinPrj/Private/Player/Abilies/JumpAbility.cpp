// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/JumpAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJumpAbility::AJumpAbility() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

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

	Parent = Buffer.ParentWitch;
	MoveComp = Buffer.MovementComp;

	if (!IsValid(Parent))
	{
		return false;
	}

	Parent->PauseTimer();
	
	bIsJumping = true;

	ResponseJumped(Buffer.ParentWitch);
	Buffer.ParentWitch->SetWitchState(EWitchStateType::Jump);
	SetActorTickEnabled(true);

	return true;
}

void AJumpAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);
	UE_LOG(LogTemp, Warning, TEXT("Call Undo Jump"));

	if (IsActorTickEnabled())
	{
		SetActorTickEnabled(false);
		ResponseEndJumped(Buffer.ParentWitch);
	}

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Idle);
}

void AJumpAbility::ResponseJumped_Implementation(ABaseWitch* ParentWitch)
{
	if (!IsValid(ParentWitch))
	{
		return;
	}

	if (ParentWitch->IsLocallyControlled())
	{
		ParentWitch->Jump();
	}

	UCapsuleComponent* HitBox = Cast<UCapsuleComponent>(ParentWitch->GetRootComponent());

	if (IsValid(HitBox))
	{
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	}
}

void AJumpAbility::ResponseEndJumped_Implementation(ABaseWitch* ParentWitch)
{
	if (!IsValid(ParentWitch))
	{
		return;
	}

	UCapsuleComponent* HitBox = Cast<UCapsuleComponent>(ParentWitch->GetRootComponent());

	if (IsValid(HitBox))
	{
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
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

void AJumpAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(MoveComp))
	{
		return;
	}

	float Direction = MoveComp->Velocity.Z;

	if (Direction < 0 && bIsJumping)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Direction Value : %f"), Direction);
		bIsJumping = false;
		ResponseEndJumped(Parent);
	}

	if (!MoveComp->IsFalling() && !bIsJumping)
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not Falling And Ended Jumping"));
		SetActorTickEnabled(false);
		Parent->RequestEndedAnim();
	}
}
