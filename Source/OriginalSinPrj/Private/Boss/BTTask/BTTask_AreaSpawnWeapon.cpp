// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_AreaSpawnWeapon.h"

#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/Object/WeaponToSpawn.h"

UBTTask_AreaSpawnWeapon::UBTTask_AreaSpawnWeapon()
{
	//공격 시작 전 딜레이(애니메이션 실행 후 딜레이)
	DelayTime = 1.0f;
	//소환 주기
	SpawnInterval = 0.5f;
	//소환 패턴 총 시간
	TotalDuration = 3.0f;
	
	SpawnWidth = 2000.0f;
	SpawnHeight = 3000.0f;
	SpawnCount = 0;
	MaxSpawnCount = 0;
	AccumulatedTime = 0.0f;
	LastSpawnTime = 0.0f;
	bIsTaskExecuting = false;
	BossController = nullptr;
	BossCharacter = nullptr;

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AreaSpawnWeapon::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;

	SpawnCount = 0;
	MaxSpawnCount = FMath::RoundToInt(TotalDuration / SpawnInterval);
	TotalDuration = TotalDuration + DelayTime;
	AccumulatedTime = 0.0f;
	LastSpawnTime = 0.0f;

	bIsTaskExecuting = true;

	// if (IsValid(BossCharacter->AreaSpawnWeaponMontage) && IsValid(BossCharacter->GetMesh()->GetAnimInstance()))
	// {
	// 	BossCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded);
	// }
	BossCharacter->PlayAreaSpawnWeaponMontage();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_AreaSpawnWeapon::TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsTaskExecuting)
	{
		FinishLatentTask(BTComponent, EBTNodeResult::Succeeded);
		return;
	}

	AccumulatedTime += DeltaSeconds;

	if (AccumulatedTime < DelayTime) return;
	
	if (AccumulatedTime - LastSpawnTime >= SpawnInterval)
	{
		LastSpawnTime = AccumulatedTime;
		FireWeapon();
		SpawnCount++;

		if (SpawnCount >= MaxSpawnCount)
		{
			bIsTaskExecuting = false;
			FinishLatentTask(BTComponent, EBTNodeResult::Succeeded);
		}
	}

	if (AccumulatedTime >= TotalDuration)
	{
		bIsTaskExecuting = false;
		FinishLatentTask(BTComponent, EBTNodeResult::Succeeded);
	}
}

void UBTTask_AreaSpawnWeapon::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("FireWeapon Called"));

	FVector TargetLocation = BossController->GetTargetPlayerPawnLocation();
	FVector SpawnLocation = FVector(0.0f, FMath::RandRange(-SpawnWidth/2, SpawnWidth/2), SpawnHeight);
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (IsValid(PoolWorldSubsystem))
	{
		AWeaponToSpawn* Weapon = PoolWorldSubsystem->SpawnWeaponToSpawn(SpawnLocation, SpawnRotation);
		if (IsValid(Weapon))
		{
			Weapon->SetTargetDirection(TargetLocation);
		}
	}
}

// void UBTTask_AreaSpawnWeapon::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
// {
// 	if (Montage == BossCharacter->AreaSpawnWeaponMontage)
// 	{
// 		if (IsValid(BossCharacter))
// 		{
// 			BossCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UBTTask_AreaSpawnWeapon::OnMontageEnded);
// 		}
// 		FireWeapon();
// 		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
// 	}
// }
