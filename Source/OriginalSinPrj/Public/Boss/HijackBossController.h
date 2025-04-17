// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HijackBossController.generated.h"

class ABossCharacter;
class UBossObjectPoolWorldSubsystem;
class AHijackBossCharacter;

UCLASS()
class ORIGINALSINPRJ_API AHijackBossController : public AAIController
{
	GENERATED_BODY()

public:
	AHijackBossController();
	
	void SwitchBattleState();

	//테스트용
	UFUNCTION(BlueprintCallable)
	bool GetBattleState() { return bIsBattleStart; }

	UFUNCTION(BlueprintCallable)
	FVector GetPlayerSpawnLocation() { return PlayerSpawnLocation; }

	FVector GetTargetPlayerPawnLocation() const { return TargetPlayerPawn->GetActorLocation(); }

protected:
	virtual void OnPossess(APawn* Possesser) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float CheckBattleStateDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float ProjectileSpawnDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float WeaponSpawnDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float CloneSpawnDelay = 0.0f;

private:
	float FindClosestPlayerDelay = 0.0f;
	bool bIsBattleStart = false;
	APawn* TargetPlayerPawn = nullptr;
	FVector TargetLocation = FVector::ZeroVector;
	FVector PlayerSpawnLocation = FVector::ZeroVector;
	AHijackBossCharacter* HijackBossCharacter = nullptr;
	ABossCharacter* BossCharacter = nullptr;
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem = nullptr;
	FTimerHandle CheckBattleStateTimerHandle;
	FTimerHandle FindClosestPlayerTimerHandle;
	FTimerHandle ProjectileTimerHandle;
	FTimerHandle WeaponTimerHandle;
	FTimerHandle RushBossTimerHandle;

	void UpdateBossFacingDirection();
	APawn* FindClosestPlayer();
	void CheckBattleState();
	void StartBattle();
	void EndBattle();

	void FireProjectile();
	void FireWeapon();
	void CloningRushBoss();
};
