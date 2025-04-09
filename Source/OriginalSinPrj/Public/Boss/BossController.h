// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "Object/RangeAttackProjectile.h"
#include "BossController.generated.h"

class UBehaviorTree;

UCLASS()
class ORIGINALSINPRJ_API ABossController : public AAIController
{
	GENERATED_BODY()

public:
	FVector GetTargetPlayerPawnLocation() const { return TargetPlayerPawn->GetActorLocation(); }
	APawn* GetTargetPlayerPawn() const { return TargetPlayerPawn; }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float FindClosestPlayerDelay = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | AI")
	UBehaviorTree* BossBehaviorTree;

private:
	APawn* TargetPlayerPawn;
	FTimerHandle FindClosestPlayerTimerHandle;
	FTimerHandle ObjectSpawnTimerHandle;

	void UpdateBossFacingDirection();
	APawn* FindClosestPlayer();
};
