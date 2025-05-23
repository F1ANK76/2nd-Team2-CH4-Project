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
	UE_LOG(LogTemp, Warning, TEXT("Excute Jump"));
	Parent = Buffer.ParentWitch;
	MoveComp = Buffer.MovementComp;

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;

	if (!IsValid(Parent))
	{
		return false;
	}

	Parent->PauseTimer();
	
	bIsJumping = true;

	Buffer.ParentWitch->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	Buffer.ParentWitch->SetWitchState(EWitchStateType::Jump);
	SetActorTickEnabled(true);
	
	return true;
}

void AJumpAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);
	UE_LOG(LogTemp, Warning, TEXT("Undo Jump"));
	if (IsActorTickEnabled())
	{
		SetActorTickEnabled(false);
		Buffer.ParentWitch->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Idle);
	UE_LOG(LogTemp, Warning, TEXT("Change Witch State To Idle"));
	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
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
	//UE_LOG(LogTemp, Warning, TEXT("MoveComp Velocity %f"), Direction);

	if (Direction < 0 && bIsJumping)
	{
		bIsJumping = false;
		Parent->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}

	if (!MoveComp->IsFalling() && !bIsJumping)
	{
		SetActorTickEnabled(false);
		Parent->RequestEndedAnim();
	}
}
