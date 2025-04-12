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

	UE_LOG(LogTemp, Warning, TEXT("%s : Excute Hit Ability"), *Parent->GetName());

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;

	Parent->PauseTimer();

	CalculateKnockTargetPos(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Hit);
	Buffer.ParentWitch->PlayAnimation(AbilityMontage);
	Buffer.MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);
	SetActorTickEnabled(true);

	Buffer.KnockGuage += Buffer.AddedGuage;

	return true;
}

void AHitAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	UE_LOG(LogTemp, Warning, TEXT("Undo Hit"));

	SetActorTickEnabled(false);
	Buffer.MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Idle);
	Buffer.ParentWitch->StopAnimation(AbilityMontage);

	KnockDistance = FVector::ZeroVector;
	AddedKnock = FVector::ZeroVector;

	CurrentTime = 0.0f;
	bIsTopHeight = false;
}

void AHitAbility::ResponseKnocked_Implementation(APawn* Target, const FVector& MoveValue)
{
	if (!IsValid(Target))
	{
		return;
	}

	if (!Target->IsLocallyControlled())
	{
		return;
	}

	Target->AddMovementInput(MoveValue, 1);
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
	StartPos = Buffer.ParentWitch->GetActorLocation();

	float DistanceY = KnockDistanceY + Buffer.KnockGuage * KnockMultiply;
	float DistanceZ = KnockDistanceZ + Buffer.KnockGuage * KnockMultiply;

	KnockDistance = FVector(0, DistanceY, DistanceZ);

	AddedKnock.Y = DistanceY / KnockTime;
	AddedKnock.Z = DistanceZ / (KnockTime / 2);

	if (HitDirection == EDirectionType::Right)
	{
		AddedKnock.Y *= -1;
	}

	UE_LOG(LogTemp, Warning, TEXT("Knock Distance : %f, %f"), DistanceY, DistanceZ);
}

void AHitAbility::OnEndedKnock()
{
	UE_LOG(LogTemp, Warning, TEXT("End Knock"));
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

	CurrentTime += DeltaTime;

	if (CurrentTime >= KnockTime)
	{
		OnEndedKnock();
		return;
	}

	FVector CurrentLocation = Parent->GetActorLocation();

	float CurrentDistanceY = FMath::Abs(CurrentLocation.Y - StartPos.Y);
	float CurrentDistanceZ = FMath::Abs(CurrentLocation.Z - StartPos.Z);

	if (CurrentDistanceY >= KnockDistance.Y)
	{
		OnEndedKnock();
		return;
	}

	if (CurrentDistanceZ >= KnockDistance.Z && !bIsTopHeight)
	{
		bIsTopHeight = true;
		AddedKnock.Z *= -1.0f;
	}

	ResponseKnocked(Parent, AddedKnock * DeltaTime * KnockSpeed);
}