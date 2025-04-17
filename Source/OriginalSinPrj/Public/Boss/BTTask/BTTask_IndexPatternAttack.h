// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IndexPatternAttack.generated.h"

class UBossObjectPoolWorldSubsystem;
class ABossController;
class ABossCharacter;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_IndexPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IndexPatternAttack();

	void SetIsExecuting(bool NewBool) {	bIsExecuting = NewBool; }
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;

	bool bIsExecuting = false;
	float Duration = 10.0f;
	float RotationOffset = 0.0f;
	float TimeElapsed = 0.0f;
	float InitialShotCooldown = 0.08f;
	float ShotCooldown = 0.08f;
};
