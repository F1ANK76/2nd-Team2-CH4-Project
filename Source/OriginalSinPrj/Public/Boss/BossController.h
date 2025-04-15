// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "Object/RangeAttackProjectile.h"
#include "BossController.generated.h"

class UBossObjectPoolWorldSubsystem;
class ADestructibleObject;
class UBehaviorTree;

UCLASS()
class ORIGINALSINPRJ_API ABossController : public AAIController
{
	GENERATED_BODY()

public:
	void SpawnDestructibleObject();

	void ActiveObjectSpawnTimer();
	void DeactiveObjectSpawnTimer();
	
	//getter
	FVector GetTargetPlayerPawnLocation() const { return TargetPlayerPawn->GetActorLocation(); }
	APawn* GetTargetPlayerPawn() const { return TargetPlayerPawn; }
	int32 GetDestructibleObjectCount() const { return DestructibleObjectCount; }
	int32 GetMaxDestructibleObject() const { return MaxDestructibleObject; }
	//setter
	void SetOneMinusDestructibleObjectCount();
	void SetDestructibleObjectCount(int32 NewNumber) { DestructibleObjectCount = NewNumber; }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float FindClosestPlayerDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float CheckBossHpDelay = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject")
	TSubclassOf<ADestructibleObject> DestructibleObjectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject | Property")
	float DestructibleObjectSpawnDelay = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject | Property")
	int32 MaxDestructibleObject = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float SpecialAttackDelay = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | AI")
	UBehaviorTree* BossBehaviorTree = nullptr;

private:
	bool bIsBattleStart = false;
	APawn* TargetPlayerPawn = nullptr;
	FTimerHandle FindClosestPlayerTimerHandle;
	FTimerHandle ObjectSpawnTimerHandle;
	FTimerHandle BossHpCheckTimerHandle;
	FTimerHandle SpecialAttackTriggerTimerHandle;

	//DestructibleObject
	int32 DestructibleObjectCount = 0;
	TArray<FName> TargetPlatformTags;
	TArray<AActor*> FoundPlatformActors;
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;

	void UpdateBossFacingDirection();
	APawn* FindClosestPlayer();
	void StartBattle();
	void EndBattle();

	void TriggerSpecialAttack();
	 void UpdateBossHpForSpecialAttack();

	//DestructibleObject
	void FindPlatforms();
};
