// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "Player/Struct/BuffDataBuffer.h"
#include "BuffComponent.generated.h"

class ABaseWitch;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGINALSINPRJ_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	void ApplyBuff(EBuffType TargetType);
	const FBuffDataBuffer& GetBuffData() const;

protected:
	virtual void BeginPlay() override;

private:
	void ApplyAttackUp();
	void ApplyAttackRangeUp();
	void ApplyAttackSpeedUp();
	void ApplyMaxManaUp();
	void ApplyAddedManaUp();
	void ApplyDefenceUp();
	void ApplyAvoidenceUp();
	void ApplyLifePointUp();
	void ApplyEnemyMaxManaDown();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedManaValue = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedKnockGaugeValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedAttackRangeValue = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedAttackSpeedValue = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedGuardPointValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AddedAvoidRateValue = 0.1f;

private:
	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	UPROPERTY()
	FBuffDataBuffer BuffData;
};
