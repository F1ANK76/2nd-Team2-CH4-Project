// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_InstantDeathPatternAttack.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"

EBTNodeResult::Type UBTTask_InstantDeathPatternAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent,
	uint8* NodeMemory)
{
	PoolWorldSubsystem = GetWorld()->GetSubsystem<UBossObjectPoolWorldSubsystem>();
	if (!IsValid(PoolWorldSubsystem)) return EBTNodeResult::Failed;

	//제거해야할 발판 생성
	SpawnPlatform();
	
	bIsTaskExecuting = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_InstantDeathPatternAttack::TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickTask(BTComponent, NodeMemory, DeltaSeconds);

	//일정 초 지난 후, 발판 남아있으면 플레이어 전부 사망하게 함
}

void UBTTask_InstantDeathPatternAttack::SpawnPlatform()
{
}
