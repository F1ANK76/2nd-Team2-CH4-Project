// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossController.generated.h"

class ABossCharacter;
class UBossObjectPoolWorldSubsystem;
class ADestructibleObject;
class UBehaviorTree;

UCLASS()
class ORIGINALSINPRJ_API ABossController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartBattle();
	//합칠 때, 플레이어 랜덤 타겟팅 위해 변경해줘야함 -> 그냥 하지 말까도 생각중입니다.
	// UFUNCTION(BlueprintCallable)
	// void StartBattle(TArray<AActor*> Players);
	UFUNCTION(BlueprintCallable)
	void EndBattle();
	
	void EnterToStunState();

	//DestructibleObject
	void SpawnDestructibleObject();
	void ActiveObjectSpawnTimer();
	void DeactiveObjectSpawnTimer();

	//즉사패턴
	void StartInstantDeathAttackTimer();

	//무기소환공격
	void StartSpawnWeaponAttack();
	
	//getter
	FVector GetTargetPlayerPawnLocation() const { return FacingTargetPlayerPawn->GetActorLocation(); }
	APawn* GetTargetPlayerPawn() const { return FacingTargetPlayerPawn; }
	int32 GetDestructibleObjectCount() const { return DestructibleObjectCount; }
	int32 GetMaxDestructibleObject() const { return MaxDestructibleObject; }
	TArray<AActor*> GetPlatformSpawnTargets() const { return PlatformSpawnTargets; }
	//setter
	void SetOneMinusDestructibleObjectCount();
	void SetOnePlusDestructibleObjectCount();
	void SetDestructibleObjectCount(int32 NewNumber) { DestructibleObjectCount = NewNumber; }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float FindClosestPlayerDelay = 0.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float CheckBossHpDelay = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float SpecialAttackDelay = 40.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | Property")
	float StunStateDelay = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss | AI")
	UBehaviorTree* BossBehaviorTree = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject")
	TSubclassOf<ADestructibleObject> DestructibleObjectClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject | Property")
	float DestructibleObjectSpawnDelay = 4.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DestructibleObject | Property")
	int32 MaxDestructibleObject = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantDeath | Property")
	float InstantDeathDelay = 20.0f;

	//SpawnWeaponAttack Property
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnWeaponAttack | Property")
	float SpawnWidth = 15000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnWeaponAttack | Property")
	float SpawnHeight = 6000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnWeaponAttack | Property")
	float SpawnDelay = 1.0f;

	//AnimMontage Delegate
	UFUNCTION()
	void OnStartMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnStartStunMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnEndStunMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnKillAllMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	UBossObjectPoolWorldSubsystem* PoolWorldSubsystem;
	UPROPERTY()
	ABossCharacter* BossCharacter = nullptr;
	UPROPERTY()
	UAnimInstance* BossAnimInstance = nullptr;
	UPROPERTY()
	TArray<ACharacter*> PlayerCharacters;
	UPROPERTY()
	TArray<AActor*> PlatformSpawnTargets;
	UPROPERTY()
	APawn* FacingTargetPlayerPawn = nullptr;
	
	//DestructibleObject Property
	UPROPERTY()
	int32 DestructibleObjectCount = 0;
	UPROPERTY()
	TArray<FName> TargetPlatformTags;
	UPROPERTY()
	TArray<AActor*> FoundPlatformActors;

	//SpawnWeaponAttack Property
	UPROPERTY()
	int32 SpawnCount = 0;
	UPROPERTY()
	int32 MaxSpawnCount = 8;
	
	bool bIsBattleStart = false;
	
	//TimerHandle
	FTimerHandle FindClosestPlayerTimerHandle;
	FTimerHandle ObjectSpawnTimerHandle;
	FTimerHandle BossHpCheckTimerHandle;
	FTimerHandle SpecialAttackTriggerTimerHandle;
	FTimerHandle InstantDeathAttackTimerHandle;
	FTimerHandle StunStateTimerHandle;
	FTimerHandle SpawnWeapontimerHandle;

	//Controller Function
	void UpdateBossFacingDirection();
	APawn* FindClosestPlayer();

	//SpecialAttack Function
	void TriggerSpecialAttack();
	void UpdateBossHpForSpecialAttack();
	void KillAllPlayerAttack();
	bool IsAnyBossPlatform(TArray<AActor*>& Actors);

	//DestructibleObject Function
	void FindPlatforms();

	//SpawnWeaponAttac
	void FireWeapon();

	void ExitStunState();
};
