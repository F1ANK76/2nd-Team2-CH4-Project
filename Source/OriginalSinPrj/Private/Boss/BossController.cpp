// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"

void ABossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FindClosestPlayerDelay = 0.2f;
	TargetPlayerPawn = nullptr;
}

void ABossController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(
			FindClosestPlayerTimerHandle,
			this,
			&ABossController::UpdateBossFacingDirection,
			FindClosestPlayerDelay,
			true
		);
	}
}

void ABossController::UpdateBossFacingDirection()
{
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());
	if (!IsValid(Boss) || !Boss->HasAuthority()) return;

	TargetPlayerPawn = FindClosestPlayer();
	if (IsValid(TargetPlayerPawn))
	{
		Boss->UpdateFacingDirection(TargetPlayerPawn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABossController::UpdateBossFacingDirection : 유효한 플레이어 감지 실패"));
		return;
	}
}

APawn* ABossController::FindClosestPlayer()
{
	APawn* ClosestPlayer = nullptr;
	float MinDistance = FLT_MAX;

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			APawn* PlayerPawn = It->Get()->GetPawn();
			if (IsValid(PlayerPawn))
			{
				float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestPlayer = PlayerPawn;
				}
			}
		}
	}
	if (IsValid(ClosestPlayer))
	{
		return ClosestPlayer;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("유효한 플레이어 감지 실패"));
		return nullptr;
	}
}