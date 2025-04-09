// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ABossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bReplicates = true;
	SetReplicateMovement(true);
}

void ABossCharacter::UpdateFacingDirection(APawn* ClosestPlayer)
{
	if (!IsValid(ClosestPlayer)) return;

	if (HasAuthority())
	{
		FVector BossLocation = GetActorLocation();
		FVector PlayerLocation = ClosestPlayer->GetActorLocation();

		float Direction = PlayerLocation.Y - BossLocation.Y;

		SetFacingDirection(Direction);
	}
}

void ABossCharacter::SetFacingDirection(float Direction)
{
	if (Direction < 0)
	{
		//왼쪽 방향
		SetActorRotation(FRotator(0.0f, -60.0f, 0.0f));
	}
	else
	{
		//오른쪽 방향
		SetActorRotation(FRotator(0.0f, 60.0f, 0.0f));
	}
}

void ABossCharacter::MulticastPlayRangeAttackMontage_Implementation()
{
	if (RangeAttackMontage && GetNetMode() != NM_DedicatedServer)
	{
		PlayAnimMontage(RangeAttackMontage);
	}
}

void ABossCharacter::MulticastPlayAreaSpawnWeaponMontage_Implementation()
{
	if (AreaSpawnWeaponMontage && GetNetMode() != NM_DedicatedServer)
	{
		PlayAnimMontage(AreaSpawnWeaponMontage);
	}
}

void ABossCharacter::MulticastPlayRushBossAttackMontage_Implementation()
{
	if (RushBossAttackMontage && GetNetMode() != NM_DedicatedServer)
	{
		PlayAnimMontage(RushBossAttackMontage);
	}
}

void ABossCharacter::PlayRangeAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayRangeAttackMontage();
	}
}

void ABossCharacter::PlayAreaSpawnWeaponMontage()
{
	if (HasAuthority())
	{
		MulticastPlayAreaSpawnWeaponMontage();
	}
}

void ABossCharacter::PlayRushBossAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayRushBossAttackMontage();
	}
}
