// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AreaSpawnWeapon.generated.h"

class ABossCharacter;
class ABossController;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_AreaSpawnWeapon : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
private:
	UPROPERTY()
	UBehaviorTreeComponent* BTComp;
	UPROPERTY()
	ABossController* BossController = nullptr;
	UPROPERTY()
	ABossCharacter* BossCharacter = nullptr;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
