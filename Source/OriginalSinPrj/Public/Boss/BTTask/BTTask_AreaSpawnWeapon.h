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

public:
	UBTTask_AreaSpawnWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& BTComponent, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaSpawnWeapon | Property")
	float DelayTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaSpawnWeapon | Property")
	float SpawnInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaSpawnWeapon | Property")
	float TotalDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaSpawnWeapon | Property")
	float SpawnWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaSpawnWeapon | Property");
	float SpawnHeight;
	
private:
	UPROPERTY()
	UBehaviorTreeComponent* BTComp;
	UPROPERTY()
	ABossController* BossController;
	UPROPERTY()
	ABossCharacter* BossCharacter;
	
	FTimerHandle SpawnWeaponTimerHandle;
	int32 SpawnCount;
	int32 MaxSpawnCount;

	float AccumulatedTime;
	float LastSpawnTime;
	bool bIsTaskExecuting;
	
	void FireWeapon();
};
