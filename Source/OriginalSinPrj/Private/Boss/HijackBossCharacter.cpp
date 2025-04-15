// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/HijackBossCharacter.h"

#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Boss/HijackBossController.h"
#include "Kismet/GameplayStatics.h"

AHijackBossCharacter::AHijackBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsDead = false;
	
	AIControllerClass = AHijackBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bReplicates = true;
	SetReplicateMovement(true);
}

void AHijackBossCharacter::MulticastPlayCastingMontage_Implementation()
{
	if (CastingMontage && GetNetMode() != NM_DedicatedServer)
	{
		PlayAnimMontage(CastingMontage);
	}
}

void AHijackBossCharacter::PlayCastingMontage()
{
	if (HasAuthority())
	{
		MulticastPlayCastingMontage();
	}
}

void AHijackBossCharacter::UpdateFacingDirection(APawn* ClosestPlayer)
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

void AHijackBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	BossCharacter = Cast<ABossCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossCharacter::StaticClass()));
	if (!IsValid(BossCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("BossController is null"));
		return;
	}
	BossController = Cast<ABossController>(BossCharacter->GetController());
	if (!IsValid(BossController))
	{
		UE_LOG(LogTemp, Error, TEXT("BossController is null"));
		return;
	}
	
	CurrentHP = BossCharacter->GetCurrentHP();
}

void AHijackBossCharacter::SetFacingDirection(float Direction)
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