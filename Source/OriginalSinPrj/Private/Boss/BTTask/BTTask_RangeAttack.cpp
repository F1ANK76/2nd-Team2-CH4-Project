// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_RangeAttack.h"
#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Boss/Object/RangeAttackProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;

	BossCharacter->PlayRangeAttackMontage();
	FireProjectile();
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_RangeAttack::FireProjectile()
{
	if (!IsValid(BossController)) return;
	
	FVector TargetLocation = BossController->GetTargetPlayerPawnLocation();
	FVector SpawnLocation = BossCharacter->GetActorLocation();
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (IsValid(PoolWorldSubsystem))
	{
		ARangeAttackProjectile* Projectile = PoolWorldSubsystem->SpawnRangeAttackProjectile(SpawnLocation, SpawnRotation);
		if (IsValid(Projectile))
		{
			Projectile->SetTargetDirection(TargetLocation);
		}
	}
}