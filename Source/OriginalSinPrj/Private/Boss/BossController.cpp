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
		if (IsValid(BossBehaviorTree))
		{
			UE_LOG(LogTemp, Warning, TEXT("Running Behavior Tree!"));
			RunBehaviorTree(BossBehaviorTree);
			GetBlackboardComponent()->SetValueAsInt("BossHpPercent", 100);
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
		PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
		PoolWorldSubsystem->SetBossReference(Cast<ABossCharacter>(GetPawn()));
		FindPlatforms();

		UpdateBossFacingDirection();

		//플레이어 찾기
		GetWorld()->GetTimerManager().SetTimer(
			FindClosestPlayerTimerHandle,
			this,
			&ABossController::UpdateBossFacingDirection,
			FindClosestPlayerDelay,
			true
		);

		//주기적으로 보스 체력 체크 및 업데이트
		GetWorld()->GetTimerManager().SetTimer(
			BossHpCheckTimerHandle,
			this,
			&ABossController::UpdateBossHpForSpecialAttack,
			CheckBossHpDelay,
			true);
		
		//40초마다 특수공격
		GetWorld()->GetTimerManager().SetTimer(
			SpecialAttackTriggerTimerHandle,
			this,
			&ABossController::TriggerSpecialAttack,
			40.0f,
			true);
	}
}

void ABossController::SetOneMinusDestructibleObjectCount()
{
	DestructibleObjectCount -= 1;
	GetBlackboardComponent()->SetValueAsInt("DestructibleObjectCount", DestructibleObjectCount);
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
APawn* ABossController::FindClosestPlayer()
{
	APawn* ClosestPlayer = nullptr;
	float MinDistance = FLT_MAX;

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), Players);

		for (AActor* Player : Players)
		{
			if (!Player->Tags.Contains("Player")) continue;

			float Distance = FVector::Dist(Player->GetActorLocation(), GetPawn()->GetActorLocation());

			if (Distance < MinDistance)
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

	bIsBattleStart = false;
	GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerHandle);
	GetWorldTimerManager().ClearTimer(ObjectSpawnTimerHandle);
	//GetWorldTimerManager().ClearTimer(SpecialAttackTriggerTimerHandle);
	GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", false);
	if (IsValid(BrainComponent))
	{
		BrainComponent->StopLogic(TEXT("Boss Battle Ended"));
	}
}

void ABossController::TriggerSpecialAttack()
{
	GetBlackboardComponent()->SetValueAsBool("bCanSpecialAttack", true);
	UE_LOG(LogTemp, Warning, TEXT("CanSpecialAttack Set"));
}

void ABossController::UpdateBossHpForSpecialAttack()
{
	ABossCharacter* BossCharacter = Cast<ABossCharacter>(GetPawn());
	if (!IsValid(BossCharacter)) return;
	
	int32 HpPercent = BossCharacter->GetCurrentHP() / BossCharacter->GetMaxHP() * 100;
	int32 PreviousHpPercent = GetBlackboardComponent()->GetValueAsInt("BossHpPercent");
	
	GetBlackboardComponent()->SetValueAsInt("BossHpPercent", HpPercent);

	// HP가 특정 임계값 아래로 떨어졌을 때 특수 공격 트리거 (최초 1회만)
	if (PreviousHpPercent > 75 && HpPercent <= 75) {
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder75AttackPlayed");
		if (!bAlreadyExecuted) {
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 75%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp75Attack", true);
		}
	}
	else if (PreviousHpPercent > 50 && HpPercent <= 50) {
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder50AttackPlayed");
		if (!bAlreadyExecuted) {
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 50%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp50Attack", true);
		}
	}
	else if (PreviousHpPercent > 25 && HpPercent <= 25) {
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder25AttackPlayed");
		if (!bAlreadyExecuted) {
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 25%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp25Attack", true);
		}
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
			if (Actor->Tags.Contains("Hijack")) continue;
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
	
	ADestructibleObject* DefaultDestructible = Cast<ADestructibleObject>(
		ADestructibleObject::StaticClass()->GetDefaultObject());
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
		ADestructibleObject* DestructibleObject = PoolWorldSubsystem->SpawnDestructibleObject(
			SpawnLocation, FRotator::ZeroRotator);
		if (IsValid(DestructibleObject))
		{
			DestructibleObject->SetBossControllerCache(this);
		}
		DestructibleObjectCount++;
		GetBlackboardComponent()->SetValueAsInt("DestructibleObjectCount", DestructibleObjectCount);
		UE_LOG(LogTemp, Warning, TEXT("DestructibleObjectCount : %d"), DestructibleObjectCount);
	}
}

void ABossController::ActiveObjectSpawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		ObjectSpawnTimerHandle,
		this,
		&ABossController::SpawnDestructibleObject,
		DestructibleObjectSpawnDelay,
		true);
}

void ABossController::DeactiveObjectSpawnTimer()
{
	GetWorldTimerManager().ClearTimer(ObjectSpawnTimerHandle);
}