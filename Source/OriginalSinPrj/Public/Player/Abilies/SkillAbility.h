// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/AttackAbility.h"
#include "SkillAbility.generated.h"

class UNiagaraComponent;

UCLASS()
class ORIGINALSINPRJ_API ASkillAbility : public AAttackAbility
{
	GENERATED_BODY()
	
public:
	ASkillAbility();
	/*virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;*/
	virtual void ExcuteSkillAttack(const FAbilityDataBuffer& Buffer);
	virtual void UndoSkillAttack(const FAbilityDataBuffer& Buffer);

protected:
	UFUNCTION(NetMulticast, Unreliable)
	void PlaySelfEffect();

	UFUNCTION(NetMulticast, Unreliable)
	void StopSelfEffect();

	virtual void BeginPlay() override;
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;

	void ApplySpawnDelayTimer(const FAbilityDataBuffer& Buffer);
	void ExcuteSpawn(const FAbilityDataBuffer& Buffer);
	void ExcuteNonDelaySpawn(const FAbilityDataBuffer& Buffer);
	void CalculateRandomDirection();
	void CalculateNextSpawnPos();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UNiagaraComponent> WitchSelfEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EPivotType EffectPivot = EPivotType::Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 ConsumeMana = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SpawnCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float EffectKeepTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SpawnDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bUseRandomPos = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bSpawnContinued = false;

protected:
	FTimerHandle EffectTimer;
	FTimerHandle SpawnTimer;
	float CurrentSpawnCount = 0;

	FVector OriginAdded = FVector::ZeroVector;
};
