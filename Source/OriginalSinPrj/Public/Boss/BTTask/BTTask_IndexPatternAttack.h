// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IndexPatternAttack.generated.h"

class ABossController;
class ABossCharacter;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_IndexPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IndexPatternAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;

private:
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
};
