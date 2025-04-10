// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/Object/DestructibleObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ABossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{
		FindClosestPlayerDelay = 0.2f;
		TargetPlayerPawn = nullptr;
		bIsBattleStart = false;
		DestructibleObjectSpawnDelay = 4.0f;
		CanSpecialAttackDelay = 40.0f;
		DestructibleObjectCount = 0;
		MaxDestructibleObject = 8;
		PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
		
		if (IsValid(BossBehaviorTree))
		{
			UE_LOG(LogTemp, Warning, TEXT("Running Behavior Tree!"));
			RunBehaviorTree(BossBehaviorTree);
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
		FindPlatforms();
		
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

//(수정필요) 보스가 있는 곳에 도달했을 때 실행되도록
//(수정필요) 플레이어 찾는 로직(지금은 Tag로 하고 있음)
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
		if (!bIsBattleStart)
		{
			StartBattle();
		}
		return ClosestPlayer;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("유효한 플레이어 감지 실패"));
		return nullptr;
	}
}

void ABossController::StartBattle()
{
	bIsBattleStart = true;
	GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", true);

	GetWorld()->GetTimerManager().SetTimer(
		ObjectSpawnTimerHandle,
		this,
		&ABossController::SpawnDestructibleObject,
		DestructibleObjectSpawnDelay,
		true);
}

void ABossController::EndBattle()
{
	ABossCharacter* BossCharacter = Cast<ABossCharacter>(GetPawn());

	GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerHandle);
	GetWorldTimerManager().ClearTimer(ObjectSpawnTimerHandle);
	GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", false);
	if (IsValid(BrainComponent))
	{
		BrainComponent->StopLogic(TEXT("Boss Battle Ended"));
	}
	
}

void ABossController::FindPlatforms()
{
	TargetPlatformTags = {FName("Ground"), FName("Platform")};

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (!IsValid(Actor)) continue;

		for (const FName& Tag : TargetPlatformTags)
		{
			if (Actor->Tags.Contains("BossPlatform")) continue;
			if (Actor->Tags.Contains(Tag))
			{
				FoundPlatformActors.Add(Actor);
			}
		}
	}
}

void ABossController::SpawnDestructibleObject()
{
	if (FoundPlatformActors.Num() == 0) return;
	if (DestructibleObjectCount == MaxDestructibleObject) return;
	
	AActor* Platform = FoundPlatformActors[FMath::RandRange(0, FoundPlatformActors.Num() - 1)];

	ADestructibleObject* DefaultDestructible = Cast<ADestructibleObject>(ADestructibleObject::StaticClass()->GetDefaultObject());
	FVector ObjectOrigin, ObjectExtent;
	DefaultDestructible->GetActorBounds(false, ObjectOrigin, ObjectExtent);
	float Margin = ObjectExtent.Y;

	FVector Origin, BoxExtent;
	Platform->GetActorBounds(true, Origin, BoxExtent);

	float MinY = Origin.Y - BoxExtent.Y + Margin;
	float MaxY = Origin.Y + BoxExtent.Y - Margin;
	float SpawnY = FMath::RandRange(MinY, MaxY);

	FVector TraceStart(Origin.X, SpawnY, Origin.Z + BoxExtent.Z + 100.0f);
	FVector TraceEnd(Origin.X, SpawnY, Origin.Z - 3000.0f);

	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		TraceStart,
		TraceEnd,
		ObjectQueryParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 2.0f);
	
	if (bHit)
	{
		FVector SpawnLocation = HitResult.ImpactPoint + FVector(0, 0, 50.0f);
		PoolWorldSubsystem->SpawnDestructibleObject(SpawnLocation, FRotator::ZeroRotator);
		DestructibleObjectCount++;
	}
}
