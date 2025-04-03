// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "BossController.h"

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
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	else
	{
		//오른쪽 방향
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
}