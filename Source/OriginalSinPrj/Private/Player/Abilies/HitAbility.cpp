// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/HitAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	Parent = Buffer.ParentWitch;
	PreMoveState = Parent->GetCharacterMovement()->GetMovementName();
	StartPosZ = Parent->GetActorLocation().Z;

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;

	Parent->PauseTimer();
	Parent->SetWitchState(EWitchStateType::Hit);
	Parent->PlayAnimation(AbilityMontage);

	CalculateKnockTargetPos(Buffer);

	Buffer.KnockGuage += Buffer.AddedGuage;
	
	ResponseOnLaunched(KnockDistance);
	SetActorTickEnabled(true);
	Parent->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	return true;
}

void AHitAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);
	//UE_LOG(LogTemp, Warning, TEXT("Undo Hit"));
	SetActorTickEnabled(false);

	Parent->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;
	bIsStartedKnock = false;
	bIsFalling = false;

	Parent->SetWitchState(EWitchStateType::Idle);
	Parent->StopAnimation(AbilityMontage);

	KnockDistance = FVector::ZeroVector;
}

void AHitAbility::ResponseOnLaunched_Implementation(const FVector& KnockbackDistance)
{
	if (!IsValid(Parent))
	{
		return;
	}

	Parent->LaunchCharacter(KnockbackDistance, false, false);
}

bool AHitAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	CalculateHitDirection(Buffer.DamageCauser->GetActorLocation(), Buffer.ParentWitch->GetActorLocation());

	if (Buffer.LastAbilities.IsEmpty())
	{
		return true;
	}

	if (!IsValid(Buffer.LastAbilities[0]))
	{
		return true;
	}

	if (Buffer.ParentWitch->GetWitchState() == EWitchStateType::Hit)
	{
		return false;
	}

	if (Buffer.LastAbilities[0]->GetAbilityType() == EAbilityType::RollAbility)
	{
		return false;
	}

	if (Buffer.LastAbilities[0]->GetAbilityType() == EAbilityType::GuardAbility)
	{
		if (HitDirection == EDirectionType::Left)
		{
			if (!Buffer.bIsLeft)
			{
				return false;
			}
		}
		else 
		{
			if (Buffer.bIsLeft)
			{
				return false;
			}
		}
	}

	Buffer.LastAbilities[0]->UndoAbility(Buffer);

	return true;
}

void AHitAbility::CalculateHitDirection(const FVector& Compare, const FVector& Target)
{
	FVector Direction = Target - Compare;

	if (Direction.Y < 0)
	{
		HitDirection = EDirectionType::Right;
	}
	else if (Direction.Y > 0)
	{
		HitDirection = EDirectionType::Left;
	}
}

void AHitAbility::CalculateKnockTargetPos(FAbilityDataBuffer& Buffer)
{
	float DistanceY = KnockDistanceY + Buffer.KnockGuage * KnockMultiply;
	float DistanceZ = KnockDistanceZ + Buffer.KnockGuage * KnockMultiply;

	if (HitDirection == EDirectionType::Right)
	{
		DistanceY *= -1;
	}

	KnockDistance = FVector(0, DistanceY, DistanceZ);
}

void AHitAbility::CalculateStartedFalling()
{
	float CurrentDirectionZ = Parent->GetActorLocation().Z - StartPosZ;

	if (CurrentDirectionZ < 0 && !bIsFalling)
	{
		if (CurrentDirectionZ <= -0.01f)
		{
			bIsFalling = true;
			Parent->SetMeshResponseToChanel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		}
	}
}

void AHitAbility::OnEndedKnock()
{
	SetActorTickEnabled(false);
	Parent->RequestEndedAnim();
}

void AHitAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(Parent))
	{
		return;
	}

	if (!IsValid(Parent->GetCharacterMovement()))
	{
		return;
	}
	
	CalculateStartedFalling();

	if (PreMoveState.Equals(Parent->GetCharacterMovement()->GetMovementName()))
	{
		return;
	}

	PreMoveState = Parent->GetCharacterMovement()->GetMovementName();

	if (PreMoveState.Equals("Falling") && !bIsStartedKnock)
	{
		bIsStartedKnock = true;
	}

	if (!Parent->GetCharacterMovement()->IsFalling() && bIsStartedKnock)
	{
		OnEndedKnock();
	}
}