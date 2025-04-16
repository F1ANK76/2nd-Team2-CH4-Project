// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_IndexPatternAttack.h"
#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"

UBTTask_IndexPatternAttack::UBTTask_IndexPatternAttack()
{
}

EBTNodeResult::Type UBTTask_IndexPatternAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	BossController = Cast<ABossController>(BTComponent.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	BossCharacter = Cast<ABossCharacter>(BossController->GetPawn());
	if (!IsValid(BossCharacter)) return EBTNodeResult::Failed;

	
	return EBTNodeResult::InProgress;
}
