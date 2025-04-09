// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ABossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{
		FindClosestPlayerDelay = 0.2f;
		TargetPlayerPawn = nullptr;

		if (IsValid(BossBehaviorTree))
		{
			UE_LOG(LogTemp, Warning, TEXT("Running Behavior Tree!"));
			RunBehaviorTree(BossBehaviorTree);
			GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Running Behavior Tree Failed!!"));
		}
	}
}

void ABossController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UpdateBossFacingDirection();
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
	if (!HasAuthority()) return;
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());
	if (!IsValid(Boss)) return;

	TargetPlayerPawn = FindClosestPlayer();
	if (IsValid(TargetPlayerPawn))
	{
		GetBlackboardComponent()->SetValueAsBool("bIsTargetPlayerSet", true);
		Boss->UpdateFacingDirection(TargetPlayerPawn);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool("bIsTargetPlayerSet", false);
	}
}

APawn* ABossController::FindClosestPlayer()
{
	APawn* ClosestPlayer = nullptr;
	float MinDistance = FLT_MAX;

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), Players);

		for(AActor* Player : Players)
		{
			if(!Player->Tags.Contains("Player")) continue;

			float Distance = FVector::Dist(Player->GetActorLocation(), GetPawn()->GetActorLocation());

			if(Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestPlayer = Cast<APawn>(Player);
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