// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "AttackAbility.generated.h"

class ABaseProjectile;

UCLASS()
class ORIGINALSINPRJ_API AAttackAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) override;
	
protected:
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	virtual void SpawnProjectile(FAbilityDataBuffer& Buffer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float DelayTime = 0.0f;
	
protected:
	UPROPERTY()
	TObjectPtr<ABaseProjectile> ProjectileObj = nullptr;
};
