// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RushBossCloneAttack.generated.h"

class ABossController;
class ABossCharacter;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_RushBossCloneAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RushBossCloneAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, EBTNodeResult::Type NodeResult) override;
	
private:
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
	UPROPERTY()
	UBehaviorTreeComponent* BTComp;
	
	float SpawnOffset;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void CloningRushBoss();
};
