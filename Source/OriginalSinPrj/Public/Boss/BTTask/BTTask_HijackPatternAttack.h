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

public:
	UBTTask_HijackPatternAttack();

	void InitialTask();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY()
	ABossController* BossController = nullptr;
	UPROPERTY()
	ABossCharacter* BossCharacter = nullptr;
	UPROPERTY()
	AHijackBossCharacter* HijackBossCharacter = nullptr;
	UPROPERTY()
	AHijackBossController* HijackBossController = nullptr;
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem = nullptr;
	UPROPERTY()
	APawn* TargetPlayer;
	UPROPERTY()
	UBehaviorTreeComponent* BTComp = nullptr;
	
	bool bIsTaskExecuting = false;
	bool bIsRegenerateDestructibleObject = false;
	
};
