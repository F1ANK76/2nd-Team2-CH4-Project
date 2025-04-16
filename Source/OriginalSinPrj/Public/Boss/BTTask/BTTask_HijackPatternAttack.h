// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HijackPatternAttack.generated.h"

class AHijackBossController;
class AHijackBossCharacter;
class UBossObjectPoolWorldSubsystem;
class ABossCharacter;
class ABossController;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_HijackPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
	UPROPERTY()
	AHijackBossCharacter* HijackBossCharacter;
	UPROPERTY()
	AHijackBossController* HijackBossController;
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;
	UPROPERTY()
	APawn* TargetPlayer;

	bool bIsTaskExecuting;
};
