// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/Object/DestructibleObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Player/BaseWitch.h"

void ABossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABossController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
		PoolWorldSubsystem->SetBossReference(Cast<ABossCharacter>(GetPawn()));
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PlatformSpawnTarget"), PlatformSpawnTargets);
		FindPlatforms();

		//일단 임시, 추후 GameMode에서 호출
		StartBattle();
	}
}

void ABossController::SetOneMinusDestructibleObjectCount()
{
	DestructibleObjectCount -= 1;
	GetBlackboardComponent()->SetValueAsInt("DestructibleObjectCount", DestructibleObjectCount);
}

void ABossController::SetOnePlusDestructibleObjectCount()
{
	DestructibleObjectCount += 1;
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
	if (!HasAuthority()) return;

	if (IsValid(BossBehaviorTree))
	{
		UE_LOG(LogTemp, Warning, TEXT("Running Behavior Tree!"));
		RunBehaviorTree(BossBehaviorTree);
		GetBlackboardComponent()->SetValueAsInt("BossHpPercent", 100);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Running Behavior Tree Failed!!"));
		return;
	}

	bIsBattleStart = true;
	GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", true);

	//파괴가능 오브젝트 스폰
	GetWorld()->GetTimerManager().SetTimer(
		ObjectSpawnTimerHandle,
		this,
		&ABossController::SpawnDestructibleObject,
		DestructibleObjectSpawnDelay,
		true);

	//플레이어 찾기
	GetWorld()->GetTimerManager().SetTimer(
		FindClosestPlayerTimerHandle,
		this,
		&ABossController::UpdateBossFacingDirection,
		FindClosestPlayerDelay,
		true
	);

	//보스 체력 체크 및 업데이트
	GetWorld()->GetTimerManager().SetTimer(
		BossHpCheckTimerHandle,
		this,
		&ABossController::UpdateBossHpForSpecialAttack,
		CheckBossHpDelay,
		true);

	//특수공격 타이머 
	GetWorld()->GetTimerManager().SetTimer(
		SpecialAttackTriggerTimerHandle,
		this,
		&ABossController::TriggerSpecialAttack,
		SpecialAttackDelay,
		true);
}

void ABossController::EndBattle()
{
	ABossCharacter* BossCharacter = Cast<ABossCharacter>(GetPawn());

	bIsBattleStart = false;
	GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerHandle);
	GetWorldTimerManager().ClearTimer(ObjectSpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(BossHpCheckTimerHandle);
	GetWorldTimerManager().ClearTimer(SpecialAttackTriggerTimerHandle);
	GetWorldTimerManager().ClearTimer(InstantDeathAttackTimerHandle);
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
	if (PreviousHpPercent > 75 && HpPercent <= 75)
	{
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder75AttackPlayed");
		if (!bAlreadyExecuted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 75%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp75Attack", true);
		}
	}
	else if (PreviousHpPercent > 50 && HpPercent <= 50)
	{
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder50AttackPlayed");
		if (!bAlreadyExecuted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 50%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp50Attack", true);
		}
	}
	else if (PreviousHpPercent > 25 && HpPercent <= 25)
	{
		bool bAlreadyExecuted = GetBlackboardComponent()->GetValueAsBool("bIsHpUnder25AttackPlayed");
		if (!bAlreadyExecuted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss HP below 25%% - Triggering special attack!"));
			GetBlackboardComponent()->SetValueAsBool("bTriggerHp25Attack", true);
		}
	}
}

void ABossController::KillAllPlayerAttack()
{
	if (!HasAuthority()) return;

	TArray<AActor*> LeftPlatforms;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PatternPlatform"), LeftPlatforms);

	if (!IsAnyBossPlatform(LeftPlatforms)) return;
	
	GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerHandle);
	GetWorldTimerManager().ClearTimer(ObjectSpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(InstantDeathAttackTimerHandle);
	GetBlackboardComponent()->SetValueAsBool("bIsBattleStart", false);
	if (IsValid(BrainComponent))
	{
		BrainComponent->StopLogic(TEXT("Boss Battle Ended"));
	}

	//즉사공격 애니메이션

	//남아있는 발판 제거
	for (AActor* Actor : LeftPlatforms)
	{
		ABossPlatform* BossPlatform = Cast<ABossPlatform>(Actor);
		if (!BossPlatform->IsHidden())
		{
			PoolWorldSubsystem->ReturnActorToPool(BossPlatform);
		}
	}
	
	//플레이어 사망처리
	for (ACharacter* PlayerCharacter : PlayerCharacters)
	{
		ABaseWitch* PlayerWitch = Cast<ABaseWitch>(PlayerCharacter);
		if (!IsValid(PlayerWitch))
		{
			UE_LOG(LogTemp, Error, TEXT("InValid PlayerCharacter"));
			return;
		}

		float Damage = FLT_MAX;
		FDamageEvent DamageEvent;

		PlayerWitch->TakeDamage(Damage, DamageEvent, this, GetPawn());
	}
}

bool ABossController::IsAnyBossPlatform(TArray<AActor*>& Actors)
{
	int32 LeftPlatformCount = 0;
	
	for (auto Platform : Actors)
	{
		ABossPlatform* BossPlatform = Cast<ABossPlatform>(Platform);
		if (!BossPlatform->IsHidden()) LeftPlatformCount++;
	}

	UE_LOG(LogTemp, Warning, TEXT("LeftPlatformCount : %d"), LeftPlatformCount);
	return LeftPlatformCount > 0 ? true : false;
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
	if (DestructibleObjectCount >= MaxDestructibleObject) return;

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

	if (bHit)
	{
		FVector SpawnLocation = HitResult.ImpactPoint + FVector(0, 0, 50.0f);
		ADestructibleObject* DestructibleObject = PoolWorldSubsystem->SpawnDestructibleObject(
			SpawnLocation, FRotator::ZeroRotator);
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

void ABossController::StartInstantDeathAttackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		InstantDeathAttackTimerHandle,
		this,
		&ABossController::KillAllPlayerAttack,
		InstantDeathDelay,
		false);
}