// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/Struct/ProjectileDataBuffer.h"
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

	virtual void ExcuteSkillAttack(const FAbilityDataBuffer& Buffer);
	virtual void UndoSkillAttack(const FAbilityDataBuffer& Buffer);

	void ExcuteAttackByType(const FAbilityDataBuffer& Buffer);
	void UndoAttackByType(const FAbilityDataBuffer& Buffer);

	void ExcuteMelleAttack(const FAbilityDataBuffer& Buffer);
	void UndoMelleAttack(const FAbilityDataBuffer& Buffer);

	void ExcuteSpawnAttack(const FAbilityDataBuffer& Buffer);
	void UndoSpawnAttack(const FAbilityDataBuffer& Buffer);
	
	void UpdateProjectileData(const FAbilityDataBuffer& Buffer);
	void CalculateProjectilePos(ABaseWitch* Parent);

	void CheckProjectilePool();
	void SpawnProjectileObj();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Attack")
	EAttackType AttackType = EAttackType::Melle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Attack")
	TSubclassOf<ABaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Melle")
	EEffectVisibleType MelleType = EEffectVisibleType::Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float MoveDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float AttackDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float VisibleDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float DeactiveDelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	float DefaultDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	FVector MoveDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	FVector AddedLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Projectile")
	EPivotType PivotType = EPivotType::Body;


protected:
	UPROPERTY()
	TArray<TObjectPtr<ABaseProjectile>> ProjectilePool;

	UPROPERTY()
	TArray<TObjectPtr<ABaseProjectile>> ActiveProjectilePool;

	UPROPERTY()
	TObjectPtr<ABaseProjectile> ProjectileObj = nullptr;

	UPROPERTY()
	FProjectileDataBuffer ProjectileData;

	FVector SpawnLocation = FVector::ZeroVector;
};
