// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/HijackBossController.h"

#include "Boss/HijackBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

/*
 BTTask에서 납치패턴의 실행여부를 판단, 종료 시 HijackBossController 비활성화, HijackBossCharacter 비활성화
 활성화시, 기본 공격(원거리, 무기소환, 돌진) 무작위 위치에서 납치된 플레이어를 향해 발사
 */

//BTTask에서 상태변화에 따라 스위칭
void AHijackBossController::SwitchBattleState()
{
	if (!HasAuthority()) return;
	
	bIsBattleStart = !bIsBattleStart;

	if (bIsBattleStart)
	{
		StartBattle();
	}
}

void AHijackBossController::OnPossess(APawn* Possesser)
{
	Super::OnPossess(Possesser);

	HijackBossCharacter = Cast<AHijackBossCharacter>(GetPawn());

	CheckBattleStateDelay = 0.2f;
	FindClosestPlayerDelay = 0.2f;
	ProjectileSpawnDelay = 0.3f;
	WeaponSpawnDelay = 0.5f;
	CloneSpawnDelay = 1.0f;
	
	PlayerSpawnLocation = HijackBossCharacter->GetActorLocation();
	PlayerSpawnLocation.Y += 3000.0f;
}

void AHijackBossController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
		if (!IsValid(PoolWorldSubsystem)) return;
		
		//배틀상태 체크(활성화/비활성화 트리거 위함)
		GetWorld()->GetTimerManager().SetTimer(
			CheckBattleStateTimerHandle,
			this,
			&AHijackBossController::CheckBattleState,
			CheckBattleStateDelay,
			true);
	}
}

void AHijackBossController::UpdateBossFacingDirection()
{
	if (!HasAuthority()) return;
	if (!IsValid(HijackBossCharacter)) return;

	TargetPlayerPawn = FindClosestPlayer();
	TargetLocation = TargetPlayerPawn->GetActorLocation();
	if (IsValid(TargetPlayerPawn))
	{
		HijackBossCharacter->UpdateFacingDirection(TargetPlayerPawn);
	}
}

APawn* AHijackBossController::FindClosestPlayer()
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

//납치패턴 진행 여부 체크
void AHijackBossController::CheckBattleState()
{
	if (!bIsBattleStart)
	{
		EndBattle();
	}
}

void AHijackBossController::StartBattle()
{
	//전투 시작 애니메이션 로직

	
	//플레이어 바라봄, 타겟플레이어 찾기
	GetWorld()->GetTimerManager().SetTimer(
		FindClosestPlayerTimerHandle,
		this,
		&AHijackBossController::UpdateBossFacingDirection,
		FindClosestPlayerDelay,
		true
	);
	//원거리 공격 투사체 생성
	GetWorld()->GetTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AHijackBossController::FireProjectile,
		ProjectileSpawnDelay,
		true);
	//무기소환
	GetWorld()->GetTimerManager().SetTimer(
		WeaponTimerHandle,
		this,
		&AHijackBossController::FireWeapon,
		WeaponSpawnDelay,
		true);
	//분신 소환
	GetWorld()->GetTimerManager().SetTimer(
		RushBossTimerHandle,
		this,
		&AHijackBossController::CloningRushBoss,
		CloneSpawnDelay,
		true);
}

void AHijackBossController::EndBattle()
{
	//타이머 초기화
	GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerHandle);
	GetWorldTimerManager().ClearTimer(ProjectileTimerHandle);
	GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
	GetWorldTimerManager().ClearTimer(RushBossTimerHandle);
}

void AHijackBossController::FireProjectile()
{
	FVector SpawnLocation = HijackBossCharacter->GetActorLocation();
	int32 RightOrLeft = FMath::RandBool() ? -1 : 1;
	SpawnLocation.Y += FMath::RandRange(100, 5000) * RightOrLeft;
	SpawnLocation.Z += FMath::RandRange(0, 5000);

	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	ARangeAttackProjectile* Projectile = PoolWorldSubsystem->SpawnRangeAttackProjectile(SpawnLocation, SpawnRotation);
	if (IsValid(Projectile))
	{
		Projectile->SetTargetDirection(TargetLocation);
	}
}

void AHijackBossController::FireWeapon()
{
	FVector SpawnLocation = HijackBossCharacter->GetActorLocation();
	int32 RightOrLeft = FMath::RandBool() ? -1 : 1;
	SpawnLocation.Y += FMath::RandRange(3000, 5000) * RightOrLeft;
	SpawnLocation.Z += FMath::RandRange(0, 5000);

	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	AWeaponToSpawn* Weapon = PoolWorldSubsystem->SpawnWeaponToSpawn(SpawnLocation, SpawnRotation);
	if (IsValid(Weapon))
	{
		Weapon->SetTargetDirection(TargetLocation);
	}
}

void AHijackBossController::CloningRushBoss()
{
	FVector SpawnLocation = HijackBossCharacter->GetActorLocation();
	int32 RightOrLeft = FMath::RandBool() ? -1 : 1;
	SpawnLocation.Y += FMath::RandRange(-5000, 5000) * RightOrLeft;
	SpawnLocation.Z += (FMath::RandBool() ? -1 : 1) * 5000;

	PoolWorldSubsystem->SpawnRushBossClone(SpawnLocation, FRotator::ZeroRotator);
}