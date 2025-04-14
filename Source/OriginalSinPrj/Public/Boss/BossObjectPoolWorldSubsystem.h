// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/BossPoolObjectDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "BossObjectPoolWorldSubsystem.generated.h"

class ADestructibleObject;
class ARushBossClone;
class ARangeAttackProjectile;
class AWeaponToSpawn;

USTRUCT()
struct FActorPoolList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> PooledActors;
};

UCLASS()
class ORIGINALSINPRJ_API UBossObjectPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	template <typename T>
	T* SpawnPooledActor(TSubclassOf<AActor> ClassToSpawn, const FVector& Location, const FRotator& Rotation);
	void ReturnActorToPool(AActor* Actor);

	ARangeAttackProjectile* SpawnRangeAttackProjectile(const FVector& Location, const FRotator& Rotation);
	AWeaponToSpawn* SpawnWeaponToSpawn(const FVector& Location, const FRotator& Rotation);
	ARushBossClone* SpawnRushBossClone(const FVector& Location, const FRotator& Rotation);
	ADestructibleObject* SpawnDestructibleObject(const FVector& Location, const FRotator& Rotation);

protected:
	UPROPERTY()
	UBossPoolObjectDataAsset* PoolConfig;

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPoolList> ObjectPools;
	//GC 방지
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AllPooledActors;
};
