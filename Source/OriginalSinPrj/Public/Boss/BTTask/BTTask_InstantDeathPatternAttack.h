// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InstantDeathPatternAttack.generated.h"

class UBossObjectPoolWorldSubsystem;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_InstantDeathPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;
	
	bool bIsTaskExecuting = false;
	int32 MaxPlatform = 8;
	int32 PlatformCount = 0;
	
	void SpawnPlatform();
};
