// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InstantDeathPatternAttack.generated.h"

class ABossCharacter;
class ABossController;
class UBossObjectPoolWorldSubsystem;

UCLASS()
class ORIGINALSINPRJ_API UBTTask_InstantDeathPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, EBTNodeResult::Type NodeResult) override;

private:
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;
	UPROPERTY()
	TArray<AActor*> PlatformSpawnTargets;
	UPROPERTY()
	UBehaviorTreeComponent* BTComp;
	
	int32 MaxPlatform = 8;
	int32 PlatformCount = 0;
	
	

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void SpawnPlatform();
};
