// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTTask/BTTask_SetRandomAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetRandomAttack::ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory)
{
	RandomValue = FMath::RandRange(1, 100);
	FName BTTaskName;
	
	if (RandomValue <= 80)
	{
		BTTaskName = "RangeAttack";
	}
	else if (RandomValue <= 90)
	{
		BTTaskName = "AreaSpawnWeapon";
	}
	else if (RandomValue <= 100)
	{
		BTTaskName = "RushBossCloneAttack";
	}

	BTComponent.GetBlackboardComponent()->SetValueAsName("ChosenAction", BTTaskName);
	return EBTNodeResult::Succeeded;
}
